#define buf_size  700
#define buf_dac_size  1400
#define sample_Rate 12000 
#define timer_period 80


#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiClient serverClient;

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
  WiFi.begin("audioap","audioap1");
  Serial.print("Connecting to wifi");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Conected to " );
  Serial.println ( "audioap" );
  Serial.print ( "IP " );
  Serial.println ( WiFi.localIP() );
}
void setup(void)
{
  Serial.begin(115200);
  pinMode(D8, OUTPUT);
  sta_mode();


  spiBegin();
    Serial.print("connecting to ");
  Serial.println("192.168.4.1");
  
  // Use WiFiClient class to create TCP connections
  const int httpPort = 80;
  if (!serverClient.connect("192.168.4.1", httpPort)) {
    Serial.println("connection failed");
    return;
  }
delay(1000);
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
