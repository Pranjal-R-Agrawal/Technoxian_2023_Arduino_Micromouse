/** Resources
  * https://docs.arduino.cc/learn/programming/bit-math
  * http://www.123seminarsonly.com/Seminar-Reports/038/59360985-Maze-Solving-Algorithms.pdf
  * https://github.com/Craga89/Micromouse/
  * https://www.geeksforgeeks.org/set-clear-and-toggle-a-given-bit-of-a-number-in-c/
*/

#include "src/CircularBufferQueue/CircularBufferQueue.h"

#define rows 16
#define cols 16

// Matrix macros
#define linearise(row, col) row* cols + col
#define delineariseRow(location) location / cols
#define delineariseCol(location) location % cols

// Wall macros
#define distance(loc1, loc2) abs(delineariseRow(loc1) - delineariseRow(loc2)) + abs(delineariseCol(loc1) - delineariseCol(loc2))
#define markWall(location, direction) floodArray[location].neighbours |= 1 << direction
#define wallExists(location, direction) bitRead(floodArray[location].neighbours, direction)

// Cell macros
#define updateDirection(currentDirection, turn) *currentDirection = (*currentDirection + turn) % 4  // Updates the passed direction
#define getNeighbourLocation(location, direction) location + cellDirectionAddition[direction] // Calculates the location of neighbour
#define getNeighbourDistance(location, direction) wallExists(location, direction) ? 255 : floodArray[getNeighbourLocation(location, direction)].flood

#define north 0
#define east 1
#define south 2
#define west 3

#define leftTurn 3
#define uTurn 2
#define rightTurn 1

struct cell {
  byte flood;
  byte neighbours;
};

cell floodArray[rows * cols];                                                                 // This array stores the flood value and neighbour data for all the cells
byte targetCells[] = { linearise(7, 7), linearise(7, 8), linearise(8, 7), linearise(8, 8) };  // This array stores the target cells
CircularBufferQueue floodQueue;                                                               // This queue stores the cells that need to be flooded

byte currentCell = linearise(0, 0);
byte leftDir = north, currentDir = east, rightDir = south;
byte cellDirectionAddition[4] = { -16, 1, 16, -1 }; // The location of a neighbouring cell can be obtained using the values in this dictionary

byte readingCellLoc, readingCellDistance, minNeighbourDistance, targetCell;

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

void flood(byte location) {
  floodQueue.enqueue(location);
  while (!floodQueue.isEmpty()) {
    readingCellLoc = *floodQueue.dequeue();
    readingCellDistance = floodArray[readingCellLoc].flood;
    minNeighbourDistance = getNeighbourDistance(readingCellLoc, north);
    for (byte i = 1; i < 4; i++) minNeighbourDistance = min(minNeighbourDistance, getNeighbourDistance(readingCellLoc, i));
    if (readingCellDistance != minNeighbourDistance + 1) {
      readingCellDistance == minNeighbourDistance + 1;
      for (byte i = 0; i < 4; i++) floodQueue.enqueue(getNeighbourLocation(readingCellLoc, i));
    }
  }
}