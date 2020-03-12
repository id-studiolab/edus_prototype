#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    A5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS   18
int NUM_LEDS_GUIDE = 9;
int NUM_LEDS_FEEDBACK = 9;

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          100
#define FRAMES_PER_SECOND  120

// buttondebounce
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastTime = millis();
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 25;    // the debounce time; increase if the output flickers


//pins
int buttonPin = 5;
int flexPin = A0;
int tiltPin = 9;

//values for the flexing
int flexValue = 0;                // the average
int offset;
int startValue;

//smoothing
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total

// lightvariables
int fade;
int brightness = 0;
int brightnessDirection = 0;
unsigned long lastUpdate = millis();
int colorValue = 110;
int guideLocation = 0;
int guideDirection = 1;
bool guideHalt = 0;
unsigned long lastStepTime;

// progress variables
int exerciseCount = 0;
unsigned long exerciseTimeout = 1000 * 60 * 20; // milliseconds * seconds * minutes
unsigned long lastExercise;
unsigned long lastLightUpdateTime = 0;

// Tweaking parameters
int flexThreshold = 10; // threshold which the user can flex the wrist before correction starts.
int lightSpeed = 20; // delay in the scanning/achievement function.
int exerciseGoal = 20; // goal
int exerciseTime = 8; // seconds, total time, so /2 for one way
int intermediateTime = 3000; // milliseconds between exercises.

void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);

  // Setup Fastled

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  pinMode(tiltPin, INPUT);
  pinMode(buttonPin, INPUT);
  FastLED.show();

  // Input the startValue with buttonPress.
  while (1) {
    if (digitalRead(buttonPin)) {
      startValue = analogRead (flexPin);
      break;
    }
  }
  delay(2000);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  Serial.println(digitalRead(tiltPin));
  flexSensor();
  lightRhytm();

  feedbackLight1();
  feedbackLight2();
  if (tiltRead() && guideHalt == 1 ) {
    lightTrace();
    guideHalt = 0;

  }

  // resetExercise();

  if (exerciseCount >= 20) {
    //   do something
  }
  resetGame();

}

void feedbackLight1() {

  int offsetStrength;
  int offsetNumber;


  if (offset >= 15) {
    offsetStrength = map(offset, 15, 100, 0, 110);
    offsetNumber = map(offset, 10, 100, 0, 4);

  }
  else {
    offsetStrength = 0;
    offsetNumber = 0;
  }

  if (offsetNumber > 4) {
    offsetNumber = 4;
  }
  if (millis() - lastLightUpdateTime >= 100) {
    lastLightUpdateTime = millis();
    Serial.println(offsetNumber);
    if (offsetNumber > 0) {
      colorValue = 110 - offsetStrength;
      if (colorValue < 0) {
        colorValue = 0;
      }
      for (int i = 1; i <= offsetNumber; i++) {
        fill_solid(leds + 4 - i, 1, CHSV(colorValue, 255, 100));
        fill_solid(leds + 4 + i, 1, CHSV(colorValue, 255, 100));
        fill_solid(leds + 4, 1, CHSV(colorValue, 255, 100));
      }
      if (offsetNumber < 4) {
        for (int i = 0; i < 4 - offsetNumber; i++) {
          fill_solid(leds + NUM_LEDS_FEEDBACK - i - 1, 1, CRGB(0, 0, 0));
          fill_solid(leds + i, 1, CRGB(0, 0, 0));
        }
      }
    }
    else {
      fill_solid(leds, NUM_LEDS_FEEDBACK, CRGB(0, 0, 0));
      fill_solid(leds + 4, 1, CHSV(110, 255, 100));;
    }

  }

  FastLED.show();
}

void feedbackLight2() {

  int offsetStrength;
  int offsetNumber;
  fill_solid(leds, NUM_LEDS_FEEDBACK, CRGB(0, 200, 200));

  if (offset >= 10) {
    offsetStrength = map(offset, 10, 100, 0, 255);
    offsetNumber = map(offset, 10, 100, 0, 4);

  }
  else {
    offsetStrength = 0;
    offsetNumber = 0;
  }
  if (offsetStrength >= 255) {
    offsetStrength = 255;
  }
  if (offsetNumber > 4) {
    offsetNumber = 4;
  }

  //fadeToBlackBy(leds, NUM_LEDS_FEEDBACK, offsetStrength);

  if (offsetNumber > 0) {

    for (int i = 0; i < offsetNumber; i++) {
      fill_solid(leds + i, 1, CRGB(0, 0, 0));
      fill_solid(leds + NUM_LEDS_FEEDBACK - i - 1, 1, CRGB(0, 0, 0));
    }
  }
  fadeToBlackBy(leds, NUM_LEDS_FEEDBACK, offsetStrength);




  FastLED.show();

}

void resetGame() {

  if (digitalRead(buttonPin)) {
    startValue = analogRead (flexPin);
    exerciseCount = 0;
  }

}

void resetExercise() {
  if (millis() - lastExercise >= exerciseTimeout) {

    exerciseCount = 0;
    lastExercise = millis();
  }

}
void flexSensor() {

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(flexPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // resetting 0 value
  if (digitalRead(buttonPin) == HIGH) {
    startValue = analogRead (flexPin);
  }

  // calculate the average:
  flexValue = total / numReadings;

  // calculate postive offset value
  offset = sqrt((startValue - flexValue) * (startValue - flexValue));
  // send it to the computer as ASCII digits

  // plotting stuff
  // Serial.print(flexValue);
  // Serial.print(",");
  //Serial.println(offset);
  // delay(10);        // delay in between reads for stability


}
void lightRhytm() {

  if (millis() - lastStepTime >= 0.5 * exerciseTime * 1000 / NUM_LEDS_GUIDE) {
    lastStepTime = millis();

    // rgb light blue-ish 64,141,255
    if (!guideHalt) {
       fill_solid(leds + NUM_LEDS_FEEDBACK, NUM_LEDS_GUIDE, CRGB::Black);
      //fadeToBlackBy(leds + NUM_LEDS_FEEDBACK, NUM_LEDS_GUIDE, 150);
      fill_solid(leds + NUM_LEDS_FEEDBACK + guideLocation, 1, CRGB(64, 141, 255));

      FastLED.show();
    }
    if (guideHalt && guideLocation == NUM_LEDS_GUIDE) {
      // do nothing
    //  fadeToBlackBy(leds + NUM_LEDS_FEEDBACK, NUM_LEDS_GUIDE, 150);
       fill_solid(leds + NUM_LEDS_FEEDBACK + guideLocation, 1, CRGB(64, 141, 255));
      Serial.println("halt");
    }
    else if (guideDirection == 1) {
      guideLocation++;
      if (guideLocation == NUM_LEDS_GUIDE ) {
        guideHalt = 1;
        if (guideLocation == NUM_LEDS_GUIDE) {
          guideDirection = -1;
        }
      }
    } else if (guideDirection == -1) {
      guideLocation--;
      if (guideLocation == 0) {
        guideDirection = 1;
        // delay(intermediateTime);  // small delay before going again    }
      }



      //fill_solid(leds,21,CHSV(i,255,255));



    }

  }
}

boolean tiltRead() {
  bool returnValue = false;
  int reading = digitalRead(tiltPin);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  lastButtonState = reading;

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {

        returnValue = true;

        lastExercise = millis();
        exerciseCount++; // add a count to the exercise
      }
    }
  }
  return returnValue;

}

void lightTrace() {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i <= NUM_LEDS_FEEDBACK - 1 ; i++) {
      fadeToBlackBy(leds, NUM_LEDS_FEEDBACK, 70);
      fill_solid(leds + i , 1, CHSV(49, 94, 100));
      FastLED.show();
      delay(lightSpeed);
    }
    for (int i = NUM_LEDS_FEEDBACK - 1 ; i >= 0; i--) {
      fadeToBlackBy(leds, NUM_LEDS_FEEDBACK, 70);
      fill_solid(leds + i , 1, CHSV(49, 94, 100));
      FastLED.show();
      delay(lightSpeed);
    }
  }
}


