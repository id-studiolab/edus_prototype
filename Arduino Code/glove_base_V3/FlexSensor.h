#ifndef _FLEXSENSOR_H_
#define _FLEXSENSOR_H_

#include <Arduino.h>
#include <math.h>

class FlexSensor {
  public:
    FlexSensor(uint8_t analogpin): _pin(analogpin) {}; 

    void begin();    
    void run();
    void takeSample();
    uint16_t getValue();

  private:
    static const uint8_t _NUMSAMPLES = 16; // Must be a power of 2
    uint8_t _pin;
    uint16_t _initialValue;
    uint16_t _cumulativeValue;
    uint8_t _sample_index;
    uint16_t _samples[_NUMSAMPLES];
};

#endif
