#include "temperature.hpp"

//#define DEBUGGING_TEMPERATURE

#ifdef DEBUGGING_TEMPERATURE
Serial pc(SERIAL_TX, SERIAL_RX);
#endif
/*
const float c[] = {
    1,
    -4.0257,
    -7.3422,
    11.819,
    18.136,
    2.1406,
    -3.5823,
    -7.5824
};
*/

const float c_NIST[] = {
    -0.176004136860E-01,
    0.389212049750E-01,
    0.185587700320E-04,
    -0.994575928740E-07,
    0.318409457190E-09,
    -0.560728448890E-12,
    0.560750590590E-15,
    -0.320207200030E-18,
    0.971511471520E-22,
    -0.121047212750E-25
};

const float d_NIST[] = {
    0.000000E+00,
    2.508355E+01,
    7.860106E-02,
    -2.503131E-01,
    8.315270E-02,
    -1.228034E-02,
    9.804036E-04,
    -4.413030E-05,
    1.057734E-06,
    -1.052755E-08
};

const float a_NIST[] = {
    0.118597600000E+00,
    -0.118343200000E-03,
    0.126968600000E+03
};

float percent2mv(float value)
{
    return value * VDDA;
}

float rtd2celsius(float analog_in)
{
    static const float alpha = 0.0;
    static float last_temp = 0.0;
    static float new_temp = 0.0;

    float voltage = percent2mv(analog_in);

    new_temp = voltage          // Voltage in mV
               / MV4CELSIUS            // Temperature in °C
               + alpha * last_temp;    // Low pass
    last_temp = new_temp;

    return new_temp;
}

/*
unsigned int thermocouple2celsius(float analog_in)
{
    static const float alpha = 0.8;
    static float last_temp = analog_in * VDDA / MV4CELSIUS;
    float new_temp;

    new_temp = analog_in * VDDA + alpha * last_temp;
    last_temp = new_temp;
    return new_temp;
}
*/

OvenTemperature::OvenTemperature(PinName RTDPin, PinName thermocouplePin)
{
    RTD = new AnalogIn(RTDPin);
    thermocouple = new AnalogIn(thermocouplePin);
}

OvenTemperature::~OvenTemperature()
{
    delete RTD;
    delete thermocouple;
}

// Oven temperature, obtained applying
// cold-junction compensation, as described
// by NIST
float OvenTemperature::temperature()
{
    //static float alpha = 0.0;
    //static float T_old = 0.0;

    float V_M = percent2mv(thermocouple->read() / AMPLI_GAIN);
    float T_CJ = rtd2celsius(RTD->read());

    //float V_M = percent2mv(0.9090909090909091 / AMPLI_GAIN);
    //float T_CJ = rtd2celsius(0.0757575757575758);

    // E = sum(i = 0 to n) c_i t^i + a0 exp(a1(t - a2) ^ 2)

    float V_CJ = c_NIST[0]
                 + c_NIST[1] * T_CJ
                 + c_NIST[2] * T_CJ * T_CJ
                 + c_NIST[3] * T_CJ * T_CJ * T_CJ
                 + c_NIST[4] * T_CJ * T_CJ * T_CJ * T_CJ
                 + c_NIST[5] * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ
                 + c_NIST[6] * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ
                 + c_NIST[7] * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ
                 + c_NIST[8] * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ
                 + c_NIST[9] * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ * T_CJ
                 + a_NIST[0] * exp(a_NIST[1] * (T_CJ - a_NIST[2]) * (T_CJ - a_NIST[2]));

    float V_HJ = V_M + V_CJ;

    // T = d_0 + d_1*E + d_2*E ^ 2 + ... + d_n*E^n,

    float T = d_NIST[0]
              + d_NIST[1] * V_HJ
              + d_NIST[2] * V_HJ * V_HJ
              + d_NIST[3] * V_HJ * V_HJ * V_HJ
              + d_NIST[4] * V_HJ * V_HJ * V_HJ * V_HJ
              + d_NIST[5] * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ
              + d_NIST[6] * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ
              + d_NIST[7] * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ
              + d_NIST[8] * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ
              + d_NIST[9] * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ * V_HJ;
    
    //T = T + alpha * T_old;
    
#ifdef DEBUGGING_TEMPERATURE
    pc.printf("V_M: %f.\n", V_M);
    pc.printf("T_CJ: %f.\n", T_CJ);
    pc.printf("V_CJ: %f.\n", V_CJ);
    pc.printf("V_HJ: %f.\n", V_HJ);
    pc.printf("T: %f.\n", T);
#endif

    //T_old = T;

    return T;
}

/*
float OvenTemperature::temperature()
{
    float V_M = percent2mv(thermocouple->read());

    pc.printf("V_M: %f.\n", V_M);

    float T_CJ = rtd2celsius(RTD->read());

    pc.printf("T_CJ: %f.\n", T_CJ);

    float V_CJ = (T_CJ * (c[1] + T_CJ * (c[2] + T_CJ * (c[3] + c[4] * T_CJ)))) /
            (1 + T_CJ * (c[5] + T_CJ * (c[6] + c[7] * T_CJ)));

    pc.printf("V_CJ: %f.\n", V_CJ);

    float V_HJ = V_M + V_CJ;

    pc.printf("V_HJ: %f.\n", V_HJ);

    float T = (V_HJ * (c[1] + V_HJ * (c[2] + V_HJ * (c[3] + c[4] * V_HJ)))) /
            (1 + V_HJ * (c[5] + V_HJ * (c[6] + c[7] * V_HJ)));

    pc.printf("T: %f.\n", T);

    return T;
}
*/