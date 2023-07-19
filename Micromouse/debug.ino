void printFloodArray() {
  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 16; j++) {
      byte flood = floodArray[linearise(i, j)].flood;
      Serial.print(flood);
      if (flood < 10) Serial.print(" , ");
      else Serial.print(", ");
    }
    Serial.println();
  }
}

void testQueue(CircularBufferQueue q) {
  Serial.print("Is empty? ");
  Serial.println(q.isEmpty() ? "Empty" : "Not Empty");
  for (byte i = 0; i < 256; i++) {
    Serial.print(i);
    Serial.print(", ");
    Serial.println(q.enqueue(i) ? "Enqueued" : "Failed");
    if (i == 255) break;
  }
  Serial.print("Does enqueue? ");
  Serial.println(q.enqueue(1) ? "Enqueued" : "Failed");
  Serial.print("Is empty? ");
  Serial.println(q.isEmpty() ? "Empty" : "Not Empty");
  while (q.peek()) {
    Serial.print(*q.dequeue());
    Serial.println(" dequeued");
  }
  Serial.print("Is empty? ");
  Serial.println(q.isEmpty() ? "Empty" : "Not Empty");
}