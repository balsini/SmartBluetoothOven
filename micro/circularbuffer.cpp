#include "circularbuffer.hpp"

template <class T>
CircularBuffer<T>::CircularBuffer()
{
    head = 0;
    tail = 0;
    count = 0;
}

template <class T>
unsigned int CircularBuffer<T>::size() const
{
    unsigned int ret;

    ret = count;

    return ret;
}

template <class T>
bool CircularBuffer<T>::empty() const
{
    bool ret;

    ret = (count == 0);

    return ret;
}

template <class T>
bool CircularBuffer<T>::full() const
{
    bool ret;

    ret = (count == CIRCULAR_BUFFER_SIZE);

    return ret;
}

template <class T>
T CircularBuffer<T>::top() const
{
    T ret;

    ret = data[tail];

    return ret;
}

template <class T>
void CircularBuffer<T>::pop()
{
    if (!empty()) {
        tail++;
        count--;
    }
}

template <class T>
void CircularBuffer<T>::push(T new_value)
{
    if (!full()) {
        data[head] = new_value;
        head++;
        count++;
    }
}