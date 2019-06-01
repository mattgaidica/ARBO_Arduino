#include "NfcTag.h"
#include <Wire.h>

NfcTag nfcTag;
int led = 13;
byte *uid;
byte uidlen = 0x00;
bool flag = false;
bool preFlag = false;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  nfcTag.init();
}

void loop() {
  Serial.println("reading NFC");
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  //  uidlen = nfcTag.getUID(uid);
  //  Serial.println(*uid,HEX);
  uidlen = nfcTag.getAFI();
  Serial.println(uidlen, HEX);

  //  flag = nfcTag.readByte(EEPROM_I2C_LENGTH-1) == 0xff?true:false;
  //  if(flag != preFlag){
  //    Serial.println("get remote NFC control signal!");
  //    if(flag == true){
  //      Serial.println("led will light up!");
  //      digitalWrite(led,HIGH);
  //    }else{
  //      Serial.println("led will turn dark!");
  //      digitalWrite(led,LOW);
  //    }
  //    preFlag = flag;
  //  }

  delay(500);
}


