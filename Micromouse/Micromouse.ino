/** Resources
  * https://docs.arduino.cc/learn/programming/bit-math
  * http://www.123seminarsonly.com/Seminar-Reports/038/59360985-Maze-Solving-Algorithms.pdf
  * https://github.com/Craga89/Micromouse/
  * https://www.geeksforgeeks.org/set-clear-and-toggle-a-given-bit-of-a-number-in-c/
*/

#include "src/CircularBufferQueue/CircularBufferQueue.h"

#define rows 16
#define cols 16

#define linearise(row, col) row* cols + col
#define delineariseRow(location) location / cols
#define delineariseCol(location) location % cols

#define distance(loc1, loc2) abs(delineariseRow(loc1) - delineariseRow(loc2)) + abs(delineariseCol(loc1) - delineariseCol(loc2))

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

cell floodArray[rows * cols];                                                                 // This array stores the flood value and neighbour data for all the cells
byte targetCells[] = { linearise(7, 7), linearise(7, 8), linearise(8, 7), linearise(8, 8) };  // This array stores the target cells
CircularBufferQueue floodQueue;                                                               // This queue stores the cells that need to be flooded

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 256; i++) {
    floodArray[i].flood = min(min(distance(i, targetCells[0]), distance(i, targetCells[1])), min(distance(i, targetCells[2]), distance(i, targetCells[3])));
    floodArray[i].neighbours = 0;  // The bot assumes that there are no walls
    if (i == 255) break;
  }
}

void loop() {
}