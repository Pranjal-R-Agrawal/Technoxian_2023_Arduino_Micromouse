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
#define getNeighbourLocation(location, direction) (byte) ((short) location + cellDirectionAddition[direction])  // Calculates the location of neighbour
#define getNeighbourDistance(location, direction) wallExists(location, direction) ? 255 : floodArray[getNeighbourLocation(location, direction)].flood

// Direction macros
#define updateDirection(currentDirection, turn) *currentDirection = (*currentDirection + turn) % 4  // Updates the passed direction

#define north 0
#define east 1
#define south 2
#define west 3

#define rightTurn 1
#define uTurn 2
#define leftTurn 3

#define leftSensor 0
#define diagonalLeftSensor 1
#define centreSensor 2
#define diagonalRightSenson 3
#define rightSensor 6

#define threshold 200

struct cell {
  byte flood;
  byte neighbours;
};

cell floodArray[rows * cols];                                                                 // This array stores the flood value and neighbour data for all the cells
byte targetCells[] = { linearise(7, 7), linearise(7, 8), linearise(8, 7), linearise(8, 8) };  // This array stores the target cells
CircularBufferQueue floodQueue;                                                               // This queue stores the cells that need to be flooded

byte currentCell = linearise(0, 0);
byte leftDir = north, currentDir = east, rightDir = south;
short cellDirectionAddition[4] = { -rows, 1, rows, -1 };  // The location of a neighbouring cell can be obtained using the values in this dictionary
byte updateDirectionTurnAmount[4] = { 0, rightTurn, uTurn, leftTurn };

byte readingCellLoc, readingCellDistance, minNeighbourDistance, targetCell, targetRelativeDirection, neighbourLocation;

void setup() {
  for (byte i = 0; i < (rows * cols); i++) {
    floodArray[i].flood = min(min(distance(i, targetCells[0]), distance(i, targetCells[1])), min(distance(i, targetCells[2]), distance(i, targetCells[3])));
    floodArray[i].neighbours = 0;
    if (delineariseRow(i) == 0) markWall(i, north);
    if (delineariseCol(i) == 0) markWall(i, west);
    if (delineariseRow(i) == (rows - 1)) markWall(i, south);
    if (delineariseCol(i) == (cols - 1)) markWall(i, east);
    if (i == 255) break;
  }
}

void loop() {
  while (currentCell != targetCells[0] && currentCell != targetCells[1] && currentCell != targetCells[2] && currentCell != targetCells[3]) {
    updateWalls();
    flood();
    updateTargetCell();
    goToTargetCell();
    currentCell = targetCell;
  }
}

void flood() {
  floodQueue.enqueue(currentCell);
  while (!floodQueue.isEmpty()) {
    readingCellLoc = *floodQueue.dequeue();
    readingCellDistance = floodArray[readingCellLoc].flood;
    minNeighbourDistance = 255;
    for (byte i = 0; i < 4; i++) minNeighbourDistance = min(minNeighbourDistance, getNeighbourDistance(readingCellLoc, i));
    if (readingCellDistance != minNeighbourDistance + 1) {
      floodArray[readingCellLoc].flood = minNeighbourDistance + 1;
      for (byte i = 0; i < 4; i++) if (!wallExists(readingCellLoc, i)) floodQueue.enqueue(getNeighbourLocation(readingCellLoc, i));
    }
  }
}

void updateTargetCell() {
  targetCell = getNeighbourLocation(currentCell, 0);
  minNeighbourDistance = getNeighbourDistance(currentCell, 0);
  for (byte i = 1; i < 4; i++) {
    readingCellLoc = getNeighbourLocation(currentCell, i);
    readingCellDistance = getNeighbourDistance(currentCell, i);
    if (readingCellDistance < minNeighbourDistance) {
      minNeighbourDistance = readingCellDistance;
      targetCell = readingCellLoc;
    }
  }
}

void goToTargetCell() {
  targetRelativeDirection = (getTargetAbsoluteDirection(targetCell, currentCell) + 4 - currentDir) % 4;
  if (targetRelativeDirection == north) {
    // motor function to go straight
  } else if (targetRelativeDirection == east) {
    // motor function to take a left-turn
  } else if (targetRelativeDirection == south) {
    // motor function to take a u-turn
  } else if (targetRelativeDirection == west) {
    // motor function to take a left-turn
  }
  updateDirection(&leftDir, updateDirectionTurnAmount[targetRelativeDirection]);
  updateDirection(&currentDir, updateDirectionTurnAmount[targetRelativeDirection]);
  updateDirection(&rightDir, updateDirectionTurnAmount[targetRelativeDirection]);
}

void updateWalls() {
  if (analogRead(leftSensor) > threshold) {
    markWall(currentCell, leftDir);
    if (checkNeighbourValidity(currentCell, leftDir)) {
      markWall(getNeighbourLocation(currentCell, leftDir), (leftDir + 2) % 4);
    }
  }
  if (analogRead(centreSensor) > threshold) {
    markWall(currentCell, currentDir);
    if (checkNeighbourValidity(currentCell, currentDir)) {
      markWall(getNeighbourLocation(currentCell, currentDir), (currentDir + 2) % 4);
    }
  }
  if (analogRead(rightSensor) > threshold) {
    markWall(currentCell, rightDir);
    if (checkNeighbourValidity(currentCell, rightDir)) {
      markWall(getNeighbourLocation(currentCell, rightDir), (rightDir + 2) % 4);
    }
  }
}

bool checkNeighbourValidity(byte location, byte direction) {
  if (direction == north) return delineariseRow(location) > 0;
  else if (direction == east) return delineariseCol(location) < (cols - 1);
  else if (direction == south) return delineariseRow(location) < (rows - 1);
  else if (direction == west) return delineariseCol(location) > 0;
}

byte getTargetAbsoluteDirection(short target, short location) {
  short diff = target - location;
  if (diff == -16) return north;
  if (diff == 1) return east;
  if (diff == 16) return south;
  if (diff == -1) return west;
}