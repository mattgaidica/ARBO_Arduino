#include "NfcTag.h"

//#define EEPROM_I2C_LENGTH 512

NfcTag nfcTag;
int status;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while(!Serial) {}
  
  status = nfcTag.init();
    // https://forum.arduino.cc/index.php?topic=347425.0
  //  Wire.begin(); // Start Wire (I2C)
  sercom3.disableWIRE(); // Disable the I2C bus
  SERCOM3->I2CM.BAUD.bit.BAUD = SystemCoreClock / ( 2 * 100000) - 1 ; // Set the I2C SCL frequency to 400kHz
  sercom3.enableWIRE(); // Restart the I2C bus
  
  if (status < 0) {
    Serial.println("NFC initialization unsuccessful");
    Serial.println("Check wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
}

void loop() {
  Serial.println("reading NFC...");
  digitalWrite(LED_BUILTIN, HIGH);
  byte memvol;
  memvol = nfcTag.getMemoryVolume(); //nfcTag.readByte(EEPROM_I2C_LENGTH-1);//nfcTag.getAFI();
  Serial.println(memvol, HEX);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);

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
}
