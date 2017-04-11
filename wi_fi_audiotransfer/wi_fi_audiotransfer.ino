#define buf_size  700
#define buf_dac_size  1400
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>


WiFiUDP udp;

int  val = 0;

uint16_t adc_buf[2][buf_size]; // ADC data buffer, double buffered
uint8_t dac_buf[buf_dac_size]; // DAC data buffer, double buffered
int current_adc_buf;
int adc_buf_pos=0;
int dac_buf_pos=0;
int send_samples_now;
const int udp_port = 2214;
const int udp_in = 2211;

void spiBegin(void)
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}
void sample_isr(void)
{

   uint16_t dac_val = (dac_buf[dac_buf_pos+1] << 8) + dac_buf[dac_buf_pos ];
  if (dac_buf_pos < buf_dac_size)
  {
    dac_buf_pos = dac_buf_pos + 2;
  }
  else
  {
    dac_val = 0;
  }
  digitalWrite(D8, LOW);
  val = SPI.transfer16(dac_val | 0x3000) & 0x0fff;
  digitalWrite(D8, HIGH);

  adc_buf[current_adc_buf][adc_buf_pos] = val;
  adc_buf_pos++;
 
  if (adc_buf_pos > buf_size) {
    
    adc_buf_pos = 0;
    current_adc_buf = !current_adc_buf;
    send_samples_now = 1;
  }
}
void sta_mode(void)
{
  Serial.println("I was built on " __DATE__ " at " __TIME__ "");
  WiFi.mode(WIFI_STA);
  WiFi.begin("Bubuka", "22011993");
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

  timer1_isr_init();
  timer1_attachInterrupt(sample_isr);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(clockCyclesPerMicrosecond() / 16 * 60); //80us = 12.5kHz sampling freq

  Serial.println("setup done");
  
}

void loop()
{
  if (send_samples_now) {
    send_samples_now = 0;
    udp.beginPacket("255.255.255.255", udp_port);
    udp.write((const uint8_t *)(&adc_buf[!current_adc_buf][0]), buf_size * 2);
    udp.endPacket();
  }
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    //Serial.printf("UDP in: %i\n", packetSize);
    int len = udp.read(dac_buf, packetSize);
    dac_buf_pos=0;
  }
}
