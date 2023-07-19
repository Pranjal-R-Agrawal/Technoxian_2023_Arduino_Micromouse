#ifndef LinkedListQueue_h
#define LinkedListQueue_h

#include "Arduino.h"

class LinkedListQueue {
  public:
    LinkedListQueue();
    ~LinkedListQueue();
    void enqueue(byte item);
    byte dequeue();
    bool isEmpty();

  private:
    typedef struct _node {
      byte item;
      _node *next;
    } _node;
    
    _node *_head;
    _node *_tail;
};

#endif