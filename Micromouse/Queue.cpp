#include <Arduino.h>
#include "Queue.h"

typedef struct _node {
  byte item;
  _node *next;
} _node;

Queue::Queue() {
  _node *_head = nullptr;
  _node *_tail = nullptr;
}

void Queue::enqueue(byte item) {
  _node *node = new _node;  // Allocate memory for a new _node object
  node->item = item;
  node->next = nullptr;

  if (!_head) {
    _head = node;
    _tail = node;
  } else {
    _tail->next = node;
    _tail = node;
  }
}

byte Queue::dequeue() {
  if (_head) {
    _node *node = _head;
    byte item = _head->item;
    _head = _head->next;
    delete node;
    return item;
  }
}

bool Queue::isEmpty() {
  return !(bool) _head;
}

Queue::~Queue() {
  while (!isEmpty()) {
    dequeue();
  }
}