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
#define getNeighbourLocation(location, direction) location + cellDirectionAddition[direction]  // Calculates the location of neighbour
#define getNeighbourDistance(location, direction) wallExists(location, direction) ? 255 : floodArray[getNeighbourLocation(location, direction)].flood

// Direction macros
#define updateDirection(currentDirection, turn) *currentDirection = (*currentDirection + turn) % 4  // Updates the passed direction
#define getTargetAbsoluteDirection(diff) (diff == -16) ? north : (diff == 1)  ? east \
                                                               : (diff == 16) ? south \
                                                               : (diff == -1) ? west \
                                                                              : west  // Determines direction from difference between cell locations

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
byte cellDirectionAddition[4] = { -16, 1, 16, -1 };  // The location of a neighbouring cell can be obtained using the values in this dictionary
byte updateDirectionTurnAmount[4] = { 0, rightTurn, uTurn, leftTurn };

byte readingCellLoc, readingCellDistance, minNeighbourDistance, targetCell, targetRelativeDirection, neighbourLocation;

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

void updateTargetCell(byte location) {
  targetCell = getNeighbourLocation(location, 0);
  minNeighbourDistance = getNeighbourDistance(location, 0);
  for (byte i = 1; i < 4; i++) {
    readingCellLoc = getNeighbourLocation(location, i);
    readingCellDistance = getNeighbourDistance(location, i);
    if (readingCellDistance < minNeighbourDistance) {
      minNeighbourDistance = readingCellDistance;
      targetCell = readingCellLoc;
    }
  }
}

void goToTargetCell(byte location) {
  targetRelativeDirection = (getTargetAbsoluteDirection(targetCell - location) + 4 - currentDir) % 4;
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

void updateWalls(byte location) {
  if (analogRead(leftSensor) > threshold) {
    markWall(location, leftDir);
    if (checkNeighbourValidity(location, leftDir)) {
      neighbourLocation = getNeighbourLocation(location, leftDir);
      markWall(neighbourLocation, (leftDir + 2) % 4);
    }
  }
  if (analogRead(centreSensor) > threshold) {
    markWall(location, currentDir);
    if (checkNeighbourValidity(location, currentDir)) {
      neighbourLocation = getNeighbourLocation(location, currentDir);
      markWall(neighbourLocation, (currentDir + 2) % 4);
    }
  }
  if (analogRead(rightSensor) > threshold) {
    markWall(location, rightDir);
    if (checkNeighbourValidity(location, rightDir)) {
      neighbourLocation = getNeighbourLocation(location, rightDir);
      markWall(neighbourLocation, (rightDir + 2) % 4);
    }
  }
}

bool checkNeighbourValidity(byte location, byte direction) {
  if (direction == north) return delineariseRow(location) > 0;
  elif (direction == east) return delineariseCol(location) < (cols - 1);
  elif (direction == south) return delineariseRow(location) < (rows - 1);
  elif (direction == west) return delineariseCol(location) > 0
}