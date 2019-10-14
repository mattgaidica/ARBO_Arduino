// can use https://www.time.gov/ to sync

#include <RTCZero.h>

#define GREEN 12 // Green LED on Pin #8
/* Change these values to set the current initial time */
byte hours = 0;
byte minutes = 0;
byte seconds = 0;
/* Change these values to set the current initial date */
const byte day = 11;
const byte month = 10;
const byte year = 19;

RTCZero rtc;    // Create RTC object
boolean clockSet = false;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // LIGHTS ON
  
  while (! Serial); // Wait until Serial is ready
  Serial.begin(115200);

  Serial.println("Let's set the clock...");
  while(clockSet == 0) {
    if(hours == 0) {
      while (Serial.available() > 0) {
        hours = getSerialInt("hours");
      }
    } else if(minutes == 0) {
      while (Serial.available() > 0) {
        minutes = getSerialInt("minutes");
      }
    } else if(seconds == 0) {
      while (Serial.available() > 0) {
        seconds = getSerialInt("seconds");
      }
    } else {
      clockSet = true;
    }
  }
  Serial.println("Setting clock!");
  rtc.begin();    // Start the RTC in 24hr mode
  rtc.setTime(hours, minutes, seconds);   // Set the time
  rtc.setDate(day, month, year);    // Set the date
  SerialOutput();
  digitalWrite(13, LOW); // LIGHTS OUT
}

void loop() {
  blink(GREEN,5); // 1s
  delay(1000);
  SerialOutput();
}

// blink out an error code, Red on pin #13 or Green on pin #8
void blink(uint8_t LED, uint8_t flashes) {
  uint8_t i;
  for (i=0; i<flashes; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
}

byte getSerialInt(char serialLabel) {
  int val = Serial.parseInt();
  serialInt = (byte)val;
  Serial.print(serialLabel + ": ");
  Serial.println(serialInt);
  return serialInt;
}

// Debbugging output of time/date
void SerialOutput() {
  Serial.print(rtc.getDay());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.print(rtc.getYear()+2000);
  Serial.print(" ");
  Serial.print(rtc.getHours());
  Serial.print(":");
  if(rtc.getMinutes() < 10)
    Serial.print('0');      // Trick to add leading zero for formatting
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  if(rtc.getSeconds() < 10)
    Serial.print('0');      // Trick to add leading zero for formatting
  Serial.println(rtc.getSeconds());
}
