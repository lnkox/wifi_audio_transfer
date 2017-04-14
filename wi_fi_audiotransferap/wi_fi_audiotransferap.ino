#define buf_size  500
#define buf_dac_size  1000
#define sample_Rate 12000 
#define timer_period 80
#define DAC_BUF_CNT 5

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
const int udp_out = 2214;
const int udp_in = 2211;
WiFiUDP udp;


void spiBegin(void)
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void setup_to_ap() // Налаштування пристрою в режим  "Точки доступу"
{


  WiFi.mode(WIFI_AP);
  WiFi.softAP("audioap","audioap1");
  Serial.println("audioap   audioap1");
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP created");
  Serial.println(myIP);
}
void setup(void)
{
  Serial.begin(115200);
  pinMode(D8, OUTPUT);
  setup_to_ap();

  udp.begin(udp_in);
  spiBegin();

 Serial.println(timer_period);
  timer1_isr_init();
  timer1_attachInterrupt(sample_isr);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(clockCyclesPerMicrosecond() / 16 *timer_period); //80us = 12.5kHz sampling freq

  Serial.println("setup done");
 
  
}

void loop()
{
  proces_audio();
  
 
}
