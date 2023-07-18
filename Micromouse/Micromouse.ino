/** Resources
  * https://docs.arduino.cc/learn/programming/bit-math
  * http://www.123seminarsonly.com/Seminar-Reports/038/59360985-Maze-Solving-Algorithms.pdf
  * https://github.com/Craga89/Micromouse/
  * https://codebender.cc/library/QueueList#QueueList.h
  * https://www.geeksforgeeks.org/set-clear-and-toggle-a-given-bit-of-a-number-in-c/
*/

#define rows 16
#define cols 16

#define linearise(row, col) row* cols + col
#define delineariseRow(location) location / cols
#define delineariseCol(location) location % cols

#define markWall(location, direction) floodArray[location].neighbours |= 1 << direction
#define wallExists(location, direction) bitRead(floodArray[location].neighbours, direction)

#define north 0
#define east 1
#define south 2
#define west 3

struct cell {
  byte flood;
  byte neighbours;
};

cell floodArray[rows * cols];

QueueList<byte> queue;

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 256; i++) {
    floodArray[i].flood = 255;
    floodArray[i].neighbours = 0;
    if (i == 255) break;
  }
}

void loop() {
}