/** Resources
  * https://docs.arduino.cc/learn/programming/bit-math
  * http://www.123seminarsonly.com/Seminar-Reports/038/59360985-Maze-Solving-Algorithms.pdf
  * https://github.com/Craga89/Micromouse/
*/

#define rows 16
#define cols 16

#define linearse(row, col) row * cols + col
#define delinearise_row(location) location / cols
#define delinearise_col(location) location % cols

#define toggleWall(location, direction) floodArray[location].neighbours |= 1 << direction
#define readWall(location, direction) bitRead(floodArray[location].neighbours, direction)

#define North 0
#define East 1
#define South 2
#define West 3

struct cell{
  byte flood;
  byte neighbours;
};

cell floodArray[rows * cols];

void setup() {
  Serial.begin(9600);
  for(byte i = 0; i < 256; i++) {
    floodArray[i].flood = 255;
    floodArray[i].neighbours = 0;
    if (i == 255) break;
  }
}

void loop() {
}