#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"
SdFat sd;

#define SPI_SPEED SD_SCK_MHZ(4)
#define chipSelect 5 //4
#define buttonPin 6
#define ledPin 8
#define FILE_BASE_NAME "BR_"
SdFile file;
const unsigned long period = 10000;
unsigned long startMillis;
int buttonState = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output:
  pinMode(buttonPin, INPUT_PULLUP);  // initialize the pushbutton pin as an input:
  //  attachInterrupt(digitalPinToInterrupt(buttonPin), writeSD, CHANGE);
  while (!Serial) {}

  Serial.print("Initializing SD card...");
  if (!sd.begin(chipSelect, SPI_SPEED)) {
    Serial.println("initialization failed!");
    return; // This return statement terminates the program.
  }
  Serial.println("initialization done.");
}

void loop() {
  if (!digitalRead(buttonPin)) {
    writeSD();
  }
}

void writeSD() {
  //  detachInterrupt(digitalPinToInterrupt(buttonPin));
  // Find an unused file name.
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.txt";
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    }
  }
  Serial.println("ISR entered...");
  startMillis = millis();
  if (file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)) {
    digitalWrite(ledPin, HIGH);
    while (millis() - startMillis < period) {
      file.print("X");
    }
    file.close();
    digitalWrite(ledPin, LOW);
    Serial.println("done.");
  }
  // close the file:
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
  delay(1000); // switch debounce in case SD doesn't open
  //  attachInterrupt(digitalPinToInterrupt(buttonPin), writeSD, CHANGE);
}
