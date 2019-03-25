#include "MillisTimer.h"  // Brett Hagman's MillisTimer library
#include "FastLED.h"      // Daniel Garcia's FastLed library v3.2.1
#include "FlexSensor.h"
#include <AceButton.h>

using namespace ace_button;
FASTLED_USING_NAMESPACE

#define REPETITIONS 3

#define PIN_LEDDATA 5
#define PIN_BUTTON A1
#define PIN_FLEXSENSOR A0

#define LED_TYPE WS2812B
#define LED_COLOR_ORDER GRB
#define NUM_LEDS 16
#define NUM_VERTLEDS 9
#define NUM_HORLEDS 7
#define LED_BRIGHTNESS 100
#define LED_FRAMES_PER_SECOND 120

#define RAMPUP_TIME 2000    /* ms */
#define HOLD_TIME 500       /* ms */
#define RAMPDOWN_TIME 6000  /* ms */

#define RAMPUP_INTERVAL (round(RAMPUP_TIME / 100))
#define HOLD_INTERVAL HOLD_TIME
#define RAMPDOWN_INTERVAL (round(RAMPDOWN_TIME / 100))

AceButton button(PIN_BUTTON);

FlexSensor glWristSensor(PIN_FLEXSENSOR);
CRGB glLEDs[NUM_LEDS];
MillisTimer glUpdateFeedbackLight = MillisTimer();
MillisTimer glUpdateActionLight = MillisTimer();
MillisTimer glFinishedLight = MillisTimer();

bool glProgramRunning = false;
uint8_t glRepetitionCount = 0;
uint8_t glActionHeight = 0;
int glActionDir = 1;

void buttonEvent(AceButton* /* button */, uint8_t, uint8_t);

void setup() {
  Serial.begin(9600);

  // Setup Fastled
  FastLED.addLeds<LED_TYPE, PIN_LEDDATA, LED_COLOR_ORDER>(glLEDs, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LED_BRIGHTNESS);
  FastLED.clear();
  pinMode(PIN_BUTTON, INPUT);
  FastLED.show();


  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(buttonEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);

  glWristSensor.begin();

  glUpdateFeedbackLight.setInterval(100);
  glUpdateFeedbackLight.expiredHandler(UpdateFeedbackLight);
  glUpdateFeedbackLight.setRepeats(0);
  glUpdateFeedbackLight.start();

  glUpdateActionLight.setInterval(RAMPUP_INTERVAL);
  glUpdateActionLight.expiredHandler(UpdateActionLight);
  glUpdateActionLight.setRepeats(0);
  glUpdateActionLight.start();

  glFinishedLight.setInterval(1000);
  glFinishedLight.expiredHandler(UpdateFinishedLight);
  glFinishedLight.setRepeats(3);
  glFinishedLight.stop();
}

void loop() {
  button.check();
  glWristSensor.run();
  glUpdateFeedbackLight.run();
  glUpdateActionLight.run();
  glFinishedLight.run();

  if (glProgramRunning) {

  } else {
    glRepetitionCount = 0;
    glActionHeight = 0;
    glActionDir = 1;
    setFeedbackStrip(0);
    setActionStrip(0);
    
    glUpdateActionLight.start();
  }
}

void UpdateFeedbackLight(MillisTimer &t)
{
  setFeedbackStrip(glWristSensor.getValue());
}

void UpdateActionLight(MillisTimer &t)
{
  setActionStrip(glActionHeight);

  glActionHeight += glActionDir;

  if (glActionHeight == 0) {
    glUpdateActionLight.setInterval(RAMPUP_INTERVAL);
    glActionDir = 1;
    glRepetitionCount++;
    if (glRepetitionCount == REPETITIONS) {
      glFinishedLight.start();
      glUpdateActionLight.stop();
    }
  }

  if (glActionHeight == 100) {
    glUpdateActionLight.setInterval(HOLD_INTERVAL);        
    glActionDir = -1;    
  }
  
  if ((glActionHeight != 100) && (glActionDir == -1)) {
    glUpdateActionLight.setInterval(RAMPDOWN_INTERVAL);    
  }
}

void UpdateFinishedLight(MillisTimer &t)
{
  static int blnk = 1;
  setActionStrip(100 * blnk);

  if (blnk == 1) blnk = 0;
  else if (blnk == 0) blnk = 1;
}

// void setFeedbackStrip(uint8_t offset)
//
// Lights-up the horizontal LEDstrip by increasing intensity
// linear to the amount of LEDs that are on. LEDs light up
// from the center outwards.
void setFeedbackStrip(uint8_t offset)
{
  uint8_t offsetStrength;
  uint8_t offsetIndex;

  if (offset > 100) offset = 100;

  if (offset >= 10) {
    offsetStrength = map(offset, 10, 100, 0, 255);
    offsetIndex = map(offset, 10, 100, 0, 4);
  }
  else {
    offsetStrength = 0;
    offsetIndex = 0;
  }

  fill_solid(glLEDs, NUM_HORLEDS, CRGB(0, 200, 200));

  for (int i = 0; i < offsetIndex; ++i) {
    fill_solid(glLEDs + i, 1, CRGB(0, 0, 0));
    fill_solid(glLEDs + NUM_HORLEDS - i - 1, 1, CRGB(0, 0, 0));
  }
  fadeToBlackBy(glLEDs, NUM_HORLEDS, offsetStrength);
  FastLED.show();
}

// void setActionStrip(uint8_t offset)
//
// Lights-up the vertical LEDstrip by increading intensity
// like a volume bar. It's base is on the bottom LED and it
// gradually "grows" towards the top.
void setActionStrip(uint8_t offset)
{
  uint8_t offsetIndex;
  uint8_t shadeStrength;

  if (offset > 100) offset = 100;

  offsetIndex = map(offset, 0, 100, NUM_VERTLEDS, 0);
  shadeStrength = map(offset % NUM_VERTLEDS, 0, NUM_VERTLEDS, 255, 0);

  fill_solid(glLEDs + NUM_HORLEDS, NUM_VERTLEDS, CRGB(0, 200, 10));

  for (int i = 0; i < offsetIndex; ++i) {
    fill_solid(glLEDs + NUM_HORLEDS + NUM_VERTLEDS - 1 - i, 1, CRGB(0, 0, 0));
  }

  FastLED.show();
}

void buttonEvent(AceButton* /* button */, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventPressed:
      break;
    case AceButton::kEventReleased:
      glProgramRunning = !glProgramRunning;
      break;
  }
}
