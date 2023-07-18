#ifndef Queue_h
#define Queue_h

#include "Arduino.h"

class Queue {
  public:
    Queue();
    ~Queue();
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