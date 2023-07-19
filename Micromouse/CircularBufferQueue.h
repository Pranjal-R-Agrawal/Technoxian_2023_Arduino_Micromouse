#ifndef CircularBufferQueue_h
#define CircularBufferQueue_h

#include "Arduino.h"

class CircularBufferQueue {
  public:
    bool enqueue(byte item);
    byte* dequeue();
    byte* peek();
    bool isEmpty();

  private:
    short start = -1;
    short end = -1;
    byte array[256];
    bool isFull();
};

#endif