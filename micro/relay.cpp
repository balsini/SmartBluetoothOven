#include "relay.hpp"

Relay::Relay(PinName pin)
{
    relay = new DigitalOut(pin);
}

Relay::~Relay()
{
    delete relay;
}

void Relay::on()
{
    relay->write(1);
}

void Relay::off()
{
    relay->write(0);
}
