#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include "mbed.h"

#define CIRCULAR_BUFFER_SIZE 32

template <class T>
class CircularBuffer {
    unsigned int head, tail, count;
    T data[CIRCULAR_BUFFER_SIZE];
public:
    CircularBuffer();
    unsigned int size() const;
    bool empty() const;
    bool full() const;
    T top() const;
    void pop();
    void push(T new_value);
};
    
#endif