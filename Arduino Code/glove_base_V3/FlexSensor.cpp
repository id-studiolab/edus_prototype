#include "FlexSensor.h"

void FlexSensor::begin() {
  pinMode(_pin, INPUT);
  _initialValue = analogRead(_pin);
  _cumulativeValue = 0;
  _sample_index = 0;
}

void FlexSensor::run() {
  static unsigned long runMillis = 0;
  static unsigned long previousRunMillis = 0;

  runMillis = millis();
  if (runMillis - previousRunMillis > 10) {
    previousRunMillis = runMillis;

    takeSample();
  }
}

void FlexSensor::takeSample() {
  _cumulativeValue -= _samples[_sample_index];
  _samples[_sample_index] = analogRead(_pin);
  _cumulativeValue += _samples[_sample_index];
  ++_sample_index &= _NUMSAMPLES - 1; // Constraint the index within its limits
}

float fscale(float, float, float, float, float, float);
uint16_t FlexSensor::getValue() {
  uint16_t flexValue = _cumulativeValue / _NUMSAMPLES;
  uint16_t linearValue = 0.4 * abs(_initialValue - flexValue);
  return linearValue * linearValue;
}
