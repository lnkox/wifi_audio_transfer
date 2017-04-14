#define buf_size  700
#define buf_dac_size  1400
#define sample_Rate 12000
#define timer_period 80


#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>



WiFiServer server(80);
WiFiClient serverClient;

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
  WiFi.softAP("audioap", "audioap1");
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


  spiBegin();
  server.begin();
  server.setNoDelay(true);
  delay(1000);
  Serial.println(timer_period);
  timer1_isr_init();
  timer1_attachInterrupt(sample_isr);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(clockCyclesPerMicrosecond() / 16 * timer_period); //80us = 12.5kHz sampling freq

  Serial.println("setup done");


}

void loop()
{
  if (server.hasClient())
  {
    if (!serverClient || !serverClient.connected())
    {
      if (serverClient) serverClient.stop();
      serverClient = server.available();
      Serial.println("New client");
    }
    WiFiClient serverCl = server.available();
    serverCl.stop();
  }




}
