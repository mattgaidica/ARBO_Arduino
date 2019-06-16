// https://github.com/drewfish/arduino-ZeroRegs

#include <ZeroRegs.h>

void setup() {
  Serial.begin(9600);
  while (! Serial) {}  // wait for serial monitor to attach
  ZeroRegOptions opts = { Serial, true };
  printZeroRegGCLK(opts);
//  printZeroRegs(opts);
}

void loop() {
  // Do nothing
}
