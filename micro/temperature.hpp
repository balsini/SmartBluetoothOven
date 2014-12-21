#ifndef TEMPERATURE_HPP
#define TEMPERATURE_HPP

#include "mbed.h"

const float VDDA = 3300; // mV
const float MV4CELSIUS = 10.0; // 1*C corresponds to 10 mV
// At 250°C, the output voltage is 3000mV.
// At that temperature, the thermocouple voltage, according to
// NIST tables, should be 10.316mV, so the amplifier's gain is
// about 290,8103916246607.
const float AMPLI_GAIN = 290.8103916246607;

float rtd2celsius(float analog_in);
float thermocouple2celsius(float analog_in);

class OvenTemperature {
    AnalogIn * RTD;
    AnalogIn * thermocouple;
    
public:
    OvenTemperature(PinName RTDPin, PinName thermocouplePin);
    ~OvenTemperature();
    float temperature();
};

#endif