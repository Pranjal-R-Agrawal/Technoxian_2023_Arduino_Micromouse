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