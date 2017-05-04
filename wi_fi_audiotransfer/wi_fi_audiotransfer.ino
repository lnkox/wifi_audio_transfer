#define buf_size  1000
#define buf_dac_size  8000
#define sample_Rate 16000
#define timer_period 60
#define tx_but D2
#define set_but D1
#define play_dl 5


#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
const int udp_out = 2211;
const int udp_in = 2211;

const char *ssid = "wifiaudio";
const char *password = "";
char modes = 0;



struct eeprom_data_t {
  char eessid[20];
  char eepass[20];
} eeprom_data;

ESP8266WebServer server(80); // порт веб сервера для налаштування
WiFiUDP udp;

void spiBegin(void)
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}


void setup_to_setmode() // Налаштування пристрою в режим  "Налаштувань"
{
  modes = 1; // Режим  "Налаштувань"
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  readSet();
  Serial.println("Server and AP created");
}
void setup_to_workmode() // Налаштування пристрою в робочий режим
{
  modes = 0; // Робочий режим
  readSet();
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(eeprom_data.eessid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(eeprom_data.eessid, eeprom_data.eepass); // підключення до WI-FI мережі заданої в налаштуваннях
  int connect_err=0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
    delay(250);
    connect_err++;
    if(connect_err>20) ESP.reset();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup(void)
{
  Serial.begin(115200);
  pinMode(D8, OUTPUT);
  pinMode(tx_but, INPUT);
  pinMode(set_but, INPUT);

  udp.begin(udp_in);
  spiBegin();
  if (digitalRead(set_but)==1)
  {
    delay(500);
    if (digitalRead(set_but)==1){ setup_to_setmode();}
  }
  if (digitalRead(set_but)!=1) {setup_to_workmode();}
  Serial.println(timer_period);
  timer1_isr_init();
  timer1_attachInterrupt(sample_isr);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(clockCyclesPerMicrosecond() / 16 * timer_period); //80us = 12.5kHz sampling freq

  Serial.println("setup done");
 }

void loop()
{
  if (modes == 1)
  {
    server.handleClient();
  }
  proces_audio();


}


