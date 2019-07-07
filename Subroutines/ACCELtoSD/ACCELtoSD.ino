#include <MPU9250.h>
#include <SPI.h>
#include <SD.h>

#define cardSelect 4

File logfile;
bool logFlag = false;

// blink out an error code, I guess we can't start with errors on SD?
void error(uint8_t errno) {
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i = errno; i < 10; i++) {
      delay(200);
    }
  }
}

MPU9250 IMU(Wire, 0x69);
int imuInt = A4;
int status;

void setup() {
  // serial to display data
  Serial.begin(115200);
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

    Serial.print("Calibrating mag...");
    status = IMU.calibrateMag();
    if (status < 0) {
      Serial.println("IMU initialization unsuccessful");
      Serial.println("Check IMU wiring or try cycling power");
      Serial.print("Status: ");
      Serial.println(status);
      while (1) {}
    }

  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  char filename[15]; // what does 15 represent???
  strcpy(filename, "/ARBO_00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i / 10;
    filename[7] = '0' + i % 10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if ( ! logfile ) {
    Serial.print("Couldnt create ");
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to ");
  Serial.println(filename);

  //  status = IMU.enableWakeOnMotion(100, MPU9250::LP_ACCEL_ODR_250HZ);
  //  Serial.println("Interrupt set");
  //  Serial.print("Status: ");
  //  Serial.println(status);
}

int acquireData() {
  logfile.println("ACCEL_X,ACCEL_Y,ACCEL_Z,GYRO_X,GYRO_Y,GYRO_Z,MAG_X,MAG_Y,MAG_Z");
  for (int i = 0; i < 1000; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    IMU.readSensor();
    logfile.print(IMU.getAccelX_mss(), 3); logfile.print(",");
    logfile.print(IMU.getAccelY_mss(), 3); logfile.print(",");
    logfile.print(IMU.getAccelZ_mss(), 3); logfile.print(",");
    logfile.print(IMU.getGyroX_rads(), 3); logfile.print(",");
    logfile.print(IMU.getGyroY_rads(), 3); logfile.print(",");
    logfile.print(IMU.getGyroZ_rads(), 3); logfile.print(",");
    logfile.print(IMU.getMagX_uT(), 3); logfile.print(",");
    logfile.print(IMU.getMagY_uT(), 3); logfile.print(",");
    logfile.println(IMU.getMagZ_uT(), 3);

    //  Serial.println(IMU.getTemperature_C(), 3);
    delay(5); // 100 Hz (without serial, minus IMU.readSensor(); aquisition time?)
    digitalWrite(LED_BUILTIN, LOW);
    delay(5);
  }
  logfile.close();
  return(1);
}

void loop() {
  if (!logFlag) {
    Serial.print("Aquiring data...");
    acquireData();
    Serial.println(" Done.");
    logFlag = true;
  }
  
  //  if (digitalRead(imuInt)) {
  //    for (int i = 0; i <= 25; i++) {
  //      digitalWrite(LED_BUILTIN, HIGH);
  //      delay(50);
  //      digitalWrite(LED_BUILTIN, LOW);
  //      delay(50);
  //    }
  //  }

  // display the data
  //  Serial.print(IMU.getAccelX_mss(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getAccelY_mss(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getAccelZ_mss(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getGyroX_rads(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getGyroY_rads(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getGyroZ_rads(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getMagX_uT(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getMagY_uT(), 3);
  //  Serial.print("\t");
  //  Serial.print(IMU.getMagZ_uT(), 3);
  //  Serial.println("\t");

  digitalWrite(LED_BUILTIN, HIGH);
  delay(250); // 100 Hz (without serial, minus IMU.readSensor(); aquisition time?)
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}
