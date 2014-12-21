#ifndef TYPEDEFINITIONS_HPP
#define TYPEDEFINITIONS_HPP

#include "mbed.h"

struct Message {
    char content[32];
};

enum Status {
    IDLE,
    WAIT_FOR_START,
    DATA_EXCHANGE
};

const char PC_CONNECTED = 'p';
const char PC_TEMP      = 't';
const char OVEN_TEMP    = 'o';
const char PC_START     = 's';
const char PC_STOP      = 'd';

#endif