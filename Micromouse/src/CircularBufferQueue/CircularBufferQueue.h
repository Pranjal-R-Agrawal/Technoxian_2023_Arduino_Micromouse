#ifndef CircularBufferQueue_h
#define CircularBufferQueue_h

#include "Arduino.h"

class CircularBufferQueue {
  public:
    CircularBufferQueue(short size);
    bool enqueue(byte item);
    byte* dequeue();
    byte* peek();
    bool isEmpty();

  private:
    short start = -1;
    short end = -1;
    short length = 0;
    byte* array;
    bool isFull();
};

#endif