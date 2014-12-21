#ifndef BLUESMIRF_HDD
#define BLUESMIRF_HDD

#include "mbed.h"

#include "typedefinitions.hpp"

extern Serial * bluesmirfHID;
extern Message btMessageBuffer; 
extern bool pendingBtMessage;
void btMessageDriver();

class BluesmirfHID {
public:
    BluesmirfHID(PinName pinTX, PinName pinRX) {
        pendingBtMessage = false;
        bluesmirfHID = new Serial(pinTX, pinRX);
        bluesmirfHID->attach(btMessageDriver);
    }
    ~BluesmirfHID() {
        delete bluesmirfHID;
    }
    bool messagePending() {
        return pendingBtMessage;
    }
    Message getMessage() {
        Message m;
        
        m = btMessageBuffer;
        pendingBtMessage = false;
        
        return m;
    }
    void sendMessage(Message m) {
        for (unsigned int i=0; i<5; ++i)
            bluesmirfHID->putc(m.content[i]);
        bluesmirfHID->putc('\0');
        bluesmirfHID->putc('\n');
    }
};

#endif