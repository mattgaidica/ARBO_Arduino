#include<SPI.h>
#include "MAX30003.h"

#define MAX30003_CS_PIN 12
#define BLINK_PIN 8
#define SD_CS_PIN 4
#define THERMO_CS_PIN 5
#define ECOG_CS_PIN 14

volatile char SPI_RX_Buff[5] ;
volatile char *SPI_RX_Buff_Ptr;
int i=0;
unsigned long uintECGraw = 0;
signed long intECGraw=0;
uint8_t DataPacketHeader[20];
uint8_t data_len = 8;
signed long ecgdata;
unsigned long data;

char SPI_temp_32b[4];
char SPI_temp_Burst[100];

void setup() {
  clock_init();
  pinMode(MAX30003_CS_PIN, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);
  pinMode(THERMO_CS_PIN, OUTPUT);
  pinMode(ECOG_CS_PIN, OUTPUT);
  digitalWrite(MAX30003_CS_PIN,HIGH); //disable MAX30003
  digitalWrite(SD_CS_PIN,HIGH); //disable SD Card
  digitalWrite(THERMO_CS_PIN,HIGH); //disable SD Card
  digitalWrite(ECOG_CS_PIN,HIGH); //disable SD Card

  pinMode(BLINK_PIN, OUTPUT);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST); 
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV32); // maybe not need, throttling for now

  MAX30003_begin();   // initialize MAX30003 
  Serial.begin(115200);
}

void loop() {
  MAX30003_Reg_Read(ECG_FIFO);

  unsigned long data0 = (unsigned long) (SPI_temp_32b[0]);
  data0 = data0 <<24;
  unsigned long data1 = (unsigned long) (SPI_temp_32b[1]);
  data1 = data1 <<16;
  unsigned long data2 = (unsigned long) (SPI_temp_32b[2]);
  data2 = data2 >>6;
  data2 = data2 & 0x03;
  
  data = (unsigned long) (data0 | data1 | data2);
  ecgdata = (signed long) (data);

  MAX30003_Reg_Read(RTOR);
  unsigned long RTOR_msb = (unsigned long) (SPI_temp_32b[0]);
 // RTOR_msb = RTOR_msb <<8;
  unsigned char RTOR_lsb = (unsigned char) (SPI_temp_32b[1]);

  unsigned long rtor = (RTOR_msb<<8 | RTOR_lsb);
  rtor = ((rtor >>2) & 0x3fff) ;

  float hr =  60 /((float)rtor*0.008); 
  unsigned int HR = (unsigned int)hr;  // type cast to int

  unsigned int RR = (unsigned int)rtor*8 ;  //8ms

  Serial.print(RTOR_msb);
  Serial.print(",");
  Serial.print(RTOR_lsb);
  Serial.print(",");
  Serial.print(rtor); 
  Serial.print(",");
  Serial.print(RR);
  Serial.print(",");
  Serial.println(HR);
  
  digitalWrite(BLINK_PIN, HIGH);
  delay(50); // was 8
  digitalWrite(BLINK_PIN, LOW);
  delay(50); // was 8
}

void MAX30003_Reg_Write (unsigned char WRITE_ADDRESS, unsigned long data)
{
 
    // now combine the register address and the command into one byte:
     byte dataToSend = (WRITE_ADDRESS<<1) | WREG;
  
     // take the chip select low to select the device:
     digitalWrite(MAX30003_CS_PIN, LOW);
     
     delay(2);
     SPI.transfer(dataToSend);   //Send register location
     SPI.transfer(data>>16);     //number of register to wr
     SPI.transfer(data>>8);      //number of register to wr
     SPI.transfer(data);      //Send value to record into register
     delay(2);
     
     // take the chip select high to de-select:
     digitalWrite(MAX30003_CS_PIN, HIGH);
}

void max30003_sw_reset(void)
{
    MAX30003_Reg_Write(SW_RST,0x000000);     
    delay(100);
}

void max30003_synch(void)
{
    MAX30003_Reg_Write(SYNCH,0x000000);
}

void MAX30003_Reg_Read(uint8_t Reg_address)
{
    uint8_t SPI_TX_Buff;
   
    digitalWrite(MAX30003_CS_PIN, LOW);
    
    SPI_TX_Buff = (Reg_address<<1 ) | RREG;
    SPI.transfer(SPI_TX_Buff); //Send register location
     
    for ( i = 0; i < 3; i++)
    {
       SPI_temp_32b[i] = SPI.transfer(0xff);
    }
  
    digitalWrite(MAX30003_CS_PIN, HIGH);
}

void MAX30003_Read_Data(int num_samples)
{
    uint8_t SPI_TX_Buff;
  
    digitalWrite(MAX30003_CS_PIN, LOW);   
  
    SPI_TX_Buff = (ECG_FIFO_BURST<<1 ) | RREG;
    SPI.transfer(SPI_TX_Buff); //Send register location
  
    for ( i = 0; i < num_samples*3; ++i)
    {
      SPI_temp_Burst[i] = SPI.transfer(0x00);
    }
    
    digitalWrite(MAX30003_CS_PIN, HIGH);  
}

void MAX30003_begin()
{    
    max30003_sw_reset();
    delay(100);
    MAX30003_Reg_Write(CNFG_GEN, 0x081007);
    delay(100);
    MAX30003_Reg_Write(CNFG_CAL, 0x720000);  // 0x700000  
    delay(100);
    MAX30003_Reg_Write(CNFG_EMUX,0x0B0000);
    delay(100);
    MAX30003_Reg_Write(CNFG_ECG, 0x805000);  // d23 - d22 : 10 for 250sps , 00:500 sps
    delay(100);
  
    MAX30003_Reg_Write(CNFG_RTOR1,0x3fc600);
    max30003_synch();
    delay(100);
}

void clock_init(void)
{
  // *using pin 11
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
