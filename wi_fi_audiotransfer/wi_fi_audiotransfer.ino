#define buf_size  300
#define buf_dac_size  600
#define sample_Rate 16000 
#define timer_period 60
#define DAC_BUF_CNT 20


#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
const int udp_out = 2241;
const int udp_in = 2211;
WiFiUDP udp;


void spiBegin(void)
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}


void sta_mode(void)
{
  Serial.println("I was built on " __DATE__ " at " __TIME__ "");
  WiFi.mode(WIFI_STA);
  WiFi.begin("Bubuka","22011993");
  Serial.print("Connecting to wifi");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Conected to " );
  Serial.println ( "Bubuka" );
  Serial.print ( "IP " );
  Serial.println ( WiFi.localIP() );
}
void setup(void)
{
  Serial.begin(115200);
  pinMode(D8, OUTPUT);
  sta_mode();

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
