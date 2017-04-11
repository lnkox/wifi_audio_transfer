
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <fs.h>

int  val=0;

void spiBegin(void)
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  //SPI.setHwCs(1);
}
void sample_isr(void)
{
  
    digitalWrite(D8,LOW);
 SPI.transfer16(val | 0x3000) & 0x0fff;
  // val=SPI.transfer16(val | 0x3000) & 0x0fff;
  digitalWrite(D8,HIGH);
}
void setup(void)
{
  Serial.begin(115200);
  spiBegin();

 timer1_isr_init();
  timer1_attachInterrupt(sample_isr);
 timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(clockCyclesPerMicrosecond() / 16 * 80); //80us = 12.5kHz sampling freq

  Serial.println("setup done");
pinMode(D8,OUTPUT);
}

void loop()
{

 //Serial.println(val );
  }
