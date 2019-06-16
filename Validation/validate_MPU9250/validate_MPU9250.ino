/*
  Basic_I2C.ino
  Brian R Taylor
  brian.taylor@bolderflight.com

  Copyright (c) 2017 Bolder Flight Systems

  https://github.com/bolderflight/MPU9250

*/

#include "MPU9250.h"

// I2C bus 0  = 0x68, 1 = 0x69
MPU9250 IMU(Wire, 0x69);
int imuInt = A4;

int status;

void setup() {
  // serial to display data
  Serial.begin(9600);
  while (!Serial) {}
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(imuInt, INPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("BOOTING...");

  // start communication with IMU
  for (int i = 0; i <= 10; i++) {
    status = IMU.begin();
    if (status == 1) {
      break;
    }
    delay(500);
    Serial.print("begin failed, status = ");
    Serial.println(status);
  }

//  for (int i = 0; i <= 10; i++) {
//    status = IMU.calibrateAccel();
//    if (status == 1) {
//      break;
//    }
//    delay(500);
//    Serial.print("calibration failed, status = ");
//    Serial.println(status);
//  }

  //  status = IMU.calibrateMag();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
    status = IMU.enableWakeOnMotion(100, MPU9250::LP_ACCEL_ODR_250HZ);
    Serial.println("Interrupt set");
    Serial.print("Status: ");
    Serial.println(status);
}

void loop() {
  //  if (digitalRead(imuInt)) {
  //    for (int i = 0; i <= 25; i++) {
  //      digitalWrite(LED_BUILTIN, HIGH);
  //      delay(50);
  //      digitalWrite(LED_BUILTIN, LOW);
  //      delay(50);
  //    }
  //  }

  digitalWrite(LED_BUILTIN, HIGH);
  IMU.readSensor();
  // display the data
  Serial.print(IMU.getAccelX_mss(), 3);
  Serial.print("\t");
  Serial.print(IMU.getAccelY_mss(), 3);
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_mss(), 3);
  Serial.print("\t");
  Serial.print(IMU.getGyroX_rads(), 3);
  Serial.print("\t");
  Serial.print(IMU.getGyroY_rads(), 3);
  Serial.print("\t");
  Serial.print(IMU.getGyroZ_rads(), 3);
  Serial.print("\t");
  Serial.print(IMU.getMagX_uT(), 3);
  Serial.print("\t");
  Serial.print(IMU.getMagY_uT(), 3);
  Serial.print("\t");
  Serial.print(IMU.getMagZ_uT(), 3);
  Serial.println("\t");
  //  Serial.println(IMU.getTemperature_C(), 3);
  delay(5);
  digitalWrite(LED_BUILTIN, LOW);
  delay(5);
}
