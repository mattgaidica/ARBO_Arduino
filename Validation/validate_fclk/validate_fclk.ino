void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  clock_init();
}

uint8_t i=0;
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void clock_init(void)
{
  // from: https://forum.arduino.cc/index.php?topic=420611.0
  REG_GCLK_GENCTRL = GCLK_GENCTRL_OE |            // Enable the GCLK output
                     GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK 4
                     GCLK_GENCTRL_SRC_XOSC32K |   // Set the clock source to the external 32.768kHz 
                     GCLK_GENCTRL_ID(2);          // Set clock source on GCLK 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  
  // Enable the port multiplexer on digital pin 6
  PORT->Group[g_APinDescription[11].ulPort].PINCFG[g_APinDescription[11].ulPin].bit.PMUXEN = 1;

  // Switch the port multiplexer to peripheral H (GCLK_IO[4])
  PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXE_H;
}
