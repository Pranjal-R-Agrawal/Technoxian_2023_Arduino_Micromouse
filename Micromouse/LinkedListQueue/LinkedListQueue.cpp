#include <Arduino.h>
#include "LinkedListQueue.h"

typedef struct _node {
  byte item;
  _node *next;
} _node;

LinkedListQueue::LinkedListQueue() {
  _node *_head = nullptr;
  _node *_tail = nullptr;
}

void LinkedListQueue::enqueue(byte item) {
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

byte LinkedListQueue::dequeue() {
  if (_head) {
    _node *node = _head;
    byte item = _head->item;
    _head = _head->next;
    delete node;
    return item;
  }
}

bool LinkedListQueue::isEmpty() {
  return !(bool) _head;
}

LinkedListQueue::~LinkedListQueue() {
  while (!isEmpty()) {
    dequeue();
  }
}