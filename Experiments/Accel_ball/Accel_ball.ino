/*
  0 : idle
  1 : accel to brightness
  2 : accel to rotation freq
  3 : sleep
*/

#include "MPU9250.h"

MPU9250 IMU(Wire, 0x69);
int curMode = 0;
bool doSerial = false;

int imuInt = A4;
int status;
int led1 = 13;
int led2 = 6;
int led3 = 5;
int ledAlt = A5;
int magCount = 0;
int magHold = 300;
int ledRot = 1;
int rotDelay;
int fadeVal = 0;
bool fadeDir = true;
int magThresh = 80;

int imuAcc;
int imuUt;

void setup() {
  // serial to display data
  if (doSerial) {
    Serial.begin(115200);
  }
  //  while (!Serial) {}
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(ledAlt, OUTPUT);
  digitalWrite(led1, false);
  digitalWrite(led2, false);
  digitalWrite(led3, false);
  digitalWrite(ledAlt, false);

  pinMode(imuInt, INPUT);
  // start communication with IMU
  for (int i = 0; i <= 10; i++) {
    status = IMU.begin();
    if (status == 1) {
      break;
    }
    delay(500);
    if (doSerial) {
      Serial.print("begin failed, status = ");
      Serial.println(status);
    }
  }
  digitalWrite(ledAlt, true);
}

void loop() {
  IMU.readSensor();
  int accx = abs(IMU.getGyroX_rads());
  int accy = abs(IMU.getGyroY_rads());
  int accz = abs(IMU.getGyroZ_rads());
//  int accx = abs(IMU.getAccelX_mss());
//  int accy = abs(IMU.getAccelY_mss());
//  int accz = abs(IMU.getAccelZ_mss());
  int magx = IMU.getMagX_uT();
  int magy = IMU.getMagY_uT();
  int magz = IMU.getMagZ_uT();
  ledRot = map(magx, -25, 25, 1, 3);
  imuUt = max3(abs(magx),abs(magy),abs(magz));
  //  Serial.println(imuUt);

  if (imuUt > magThresh) {
    magCount++;
  } else {
    magCount = 0;
  }
  if (magCount > magHold) {
    curMode++;
    if (curMode == 4) {
      curMode = 0;
    }
    changeMode(ledAlt);
    magCount = 0;
    delay(250);
  }

  Serial.println(curMode);

  if (fadeDir) {
    fadeVal++;
  } else {
    fadeVal--;
  }
  if (fadeVal > 255) {
    fadeDir = false;
  }
  if (fadeVal == 0) {
    fadeDir = true;
  }

  if (curMode == 0) {
    digitalWrite(ledAlt, HIGH);
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
  } else if (curMode == 1) {
    int led1Val = map(accx, 0, 20, 0, 255);
    int led2Val = map(accy, 0, 20, 0, 255);
    int led3Val = map(accz, 0, 20, 0, 255);
    digitalWrite(ledAlt, LOW);
    analogWrite(led1, led1Val);
    analogWrite(led2, led2Val);
    analogWrite(led3, led3Val);
  } else if (curMode == 2) {
    digitalWrite(ledAlt, LOW);
    if (ledRot == 1) {
      analogWrite(led1, 255);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
    } else if (ledRot == 2) {
      analogWrite(led1, 0);
      analogWrite(led2, 255);
      analogWrite(led3, 0);
    } else {
      analogWrite(led1, 0);
      analogWrite(led2, 0);
      analogWrite(led3, 255);
    }
    if (magCount == 0) {
//      ledRot++;
      delay(50);
    }
//    if (ledRot == 4) {
//      ledRot = 1;
//    }
  } else {
    digitalWrite(ledAlt, LOW);
    analogWrite(led1, fadeVal);
    analogWrite(led2, fadeVal);
    analogWrite(led3, fadeVal);
  }
}

void changeMode(int ledAlt) {
  digitalWrite(ledAlt, HIGH);
  delay(50);
  digitalWrite(ledAlt, LOW);
  delay(50);
}

int max3(int a, int b, int c) {
  int maxguess;
  maxguess = max(a, b); // biggest of A and B
  maxguess = max(maxguess, c);  // but maybe C is bigger?
  return (maxguess);
}
int max3id(int a, int b, int c) {
  int maxId;
  if (a > b) {
    if (a > c) {
      return (1);
    } else {
      return (3);
    }
  } else {
    if (b > c) {
      return (2);
    } else {
      return (3);
    }
  }
}
