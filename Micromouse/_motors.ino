#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include <SparkFun_TB6612.h>

#define AIN1 5
#define BIN1 6
#define AIN2 4
#define BIN2 7
#define PWMA 9
#define PWMB 10
#define STBY 19

const int offsetA = 1;
const int offsetB = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

long distanceTravelled;

int P, D, I, previousError, PIDvalue, error;
int lsp = 100;
int rsp = 100;
float Kp = 1;
float Kd = 10;
float Ki = 0;

int minValues[7], maxValues[7], threshold[7], sensorValue[7], sensorArray[7];
int wallValues[5] = { 80, 80, 200, 80, 80 };

double encStart1 = 0;
double encStart2 = 0;

void moveForward(long blocks, int speed) {
  float Kp = 0.5;
  float Kd = 10;
  float Ki = 0;

  long steps = blocks * 1500;
  float currentSpeed = 30;
  encStart1 = myEnc1.read();
  encStart2 = myEnc2.read();

  while (newPosition1 < encStart1 + steps) {
    encUpdate();
    readWall();
    if (abs(encStart1 + steps - newPosition1) < 500) {
      if (currentSpeed > 35) currentSpeed = currentSpeed - 0.2;
    } else if (currentSpeed < speed) {
      currentSpeed = currentSpeed + 0.05;
    }

    int error = (newPosition1 - encStart1) - (newPosition2 - encStart2);
    // if (analogRead(6) > 30 && analogRead(0) > 30) error += (analogRead(6) - analogRead(0) - 10) * 0.75;
    P = error;
    I = I + error;
    D = error - previousError;

    PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
    previousError = error;
    int wallError = 0.5 * (sensorValue[0] - sensorValue[6] + 50);  // 50 is offset between left and right side- right being greater
    if (sensorValue[0] < 25 || sensorValue[6] < 25) wallError = 0;

    lsp = currentSpeed - PIDvalue + wallError;
    rsp = currentSpeed + PIDvalue - wallError;

    if (lsp > 200) lsp = 200;
    if (lsp < 0) lsp = 0;
    if (rsp > 200) rsp = 200;
    if (rsp < 0) rsp = 0;

    motor1.drive(lsp);
    motor2.drive(rsp);
  }
  motor1.drive(0);
  motor2.drive(0);
}

void turn(int angle, int speed) {
  float currentSpeed = 30;
  float steps = abs(angle) * 4.5;
  long endPos;
  int error;
  encStart1 = myEnc1.read();
  encStart2 = myEnc2.read();

  if (angle > 0) endPos = encStart1 + steps;
  else endPos = encStart1 - steps;

  while (newPosition1 != endPos) {
    encUpdate();

    if (abs(endPos - newPosition1) < 500) {
      if (currentSpeed > 35) currentSpeed = currentSpeed - 0.2;
    } else {
      currentSpeed = currentSpeed + 0.05;
      if (currentSpeed > speed) currentSpeed = speed;
    }

    error = abs(newPosition1 - encStart1) - abs(encStart2 - newPosition2);
    P = error;
    I = I + error;
    D = error - previousError;

    PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
    previousError = error;

    lsp = currentSpeed - PIDvalue;
    rsp = currentSpeed + PIDvalue;

    if (lsp > 200) lsp = 200;
    if (lsp < 0) lsp = 0;
    if (rsp > 200) rsp = 200;
    if (rsp < 0) rsp = 0;

    if (angle > 0) {
      motor1.drive(lsp);
      motor2.drive(-rsp);
    } else {
      motor1.drive(-lsp);
      motor2.drive(rsp);
    }
  }
  motor1.drive(0);
  motor2.drive(0);
}
void alignFront() {
  float speedFactor = 1;
  int lrOffset = 0;
  int alignUpperSpeed = 60;
  for (int i = 0; i < 2500; i++) {
    int y = wallValues[2] - analogRead(2);
    int x = analogRead(1) - analogRead(3) - lrOffset;
    x *= 0.5;
    int leftSpeed = speedFactor * (y - x);
    int rightSpeed = speedFactor * (y + x);

    if (leftSpeed < -alignUpperSpeed) leftSpeed = -alignUpperSpeed;
    if (leftSpeed > alignUpperSpeed) leftSpeed = alignUpperSpeed;
    if (rightSpeed < -alignUpperSpeed) rightSpeed = -alignUpperSpeed;
    if (rightSpeed > alignUpperSpeed) rightSpeed = alignUpperSpeed;

    motor1.drive(leftSpeed);
    motor2.drive(rightSpeed);
  }
  motor1.drive(0);
  motor2.drive(0);
  /*  Serial.print("y=  ");
  Serial.print(y);
  Serial.print("  x=  ");
  Serial.println(x);
  */
}

void encUpdate() {
  newPosition1 = myEnc1.read();
  newPosition2 = myEnc2.read();

  if (newPosition1 != oldPosition1) {
    oldPosition1 = newPosition1;
  }
  if (newPosition2 != oldPosition2) {
    oldPosition2 = newPosition2;
  }
}
void resetEnc() {
  newPosition1 = 0;
  oldPosition1 = -999;
  newPosition2 = 0;
  oldPosition2 = -999;
}

void calibrate() {
  for (int i = 0; i < 7; i++) {
    if (i != 4 && i != 5) {
      minValues[i] = analogRead(i);
      maxValues[i] = analogRead(i);
    }
  }

  for (int i = 0; i < 5000; i++) {
    motor1.drive(50);
    motor2.drive(-50);

    for (int i = 0; i < 7; i++) {
      if (i != 4 && i != 5) {
        if (analogRead(i) < minValues[i]) {
          minValues[i] = analogRead(i);
        }
        if (analogRead(i) > maxValues[i]) {
          maxValues[i] = analogRead(i);
        }
      }
    }
  }

  for (int i = 0; i < 7; i++) {
    if (i != 4 && i != 5) {
      threshold[i] = (minValues[i] + maxValues[i]) / 2;
      Serial.print(threshold[i]);
      Serial.print(" ");
    }
  }
  Serial.println();

  motor1.drive(0);
  motor2.drive(0);
}

void readWall() {
  for (int i = 0; i < 7; i++) {
    if (i != 4 && i != 5) {
      //  sensorValue[i] = map(analogRead(i), minValues[i], maxValues[i], 0, 1000);
      //  sensorValue[i] = constrain(sensorValue[i], 0, 1000);
      sensorValue[i] = analogRead(i);
    }
  }
}
