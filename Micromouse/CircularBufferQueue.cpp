#include <Arduino.h>
#include "CircularBufferQueue.h"

bool CircularBufferQueue::enqueue(byte item) {
  if (isFull()) return false;
  else if (isEmpty()) start = end = 0;
  else end = (end + 1) % 256;
  array[end] = item;
  return true;
}

byte* CircularBufferQueue::dequeue() {
  if (isEmpty()) return nullptr;
  byte *item = &array[start];
  if (start == end) start = end = -1;
  else start = (start + 1) % 256;
  return item;
}

byte* CircularBufferQueue::peek() {
  if (isEmpty()) return nullptr;
  return &array[start];
}

bool CircularBufferQueue::isEmpty() {
  return start == -1;
}

bool CircularBufferQueue::isFull() {
  return start == (end + 1) % 256;
}