#ifndef RELAY_HPP
#define RELAY_HPP

#include "mbed.h"

class Relay {
    DigitalOut * relay;
public:
    Relay(PinName pin);
    ~Relay();
    void on();
    void off();
};

#endif