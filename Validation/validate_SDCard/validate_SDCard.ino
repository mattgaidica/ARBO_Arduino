#include <SPI.h>
#include <SD.h>

// Set the pins used
#define cardSelect 4

File logfile;

// blink out an error code
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

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("\r\nAnalog logger test");
  pinMode(13, OUTPUT);


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  char filename[15];
  strcpy(filename, "/ANALOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i / 10;
    filename[8] = '0' + i % 10;
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

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.println("Ready!");

  for (int i = 0; i < 10; i++) {
    digitalWrite(8, HIGH);
    logfile.print("A0 = "); logfile.println(analogRead(0));
    Serial.print("A0 = "); Serial.println(analogRead(0));
    Serial.println(i);
    digitalWrite(8, LOW);

    delay(10);
  }
  logfile.close(); // can't forget to do this, example doesn't
}


void loop() {
}
