int  val = 0;
uint16_t adc_buf[2][buf_size]; // ADC data buffer, double buffered
uint8_t dac_buf[2][buf_dac_size]; // DAC data buffer, double buffered
int current_adc_buf=0;
int current_dac_buf=0;
int adc_buf_pos=0;
int dac_buf_pos=0;
int send_samples_now;


void spiBegin(void)
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}
void sample_isr(void)
{

   uint16_t dac_val = (dac_buf[current_dac_buf][dac_buf_pos+1] << 8) + dac_buf[current_dac_buf][dac_buf_pos ];
  if (dac_buf_pos < buf_dac_size)
  {
    dac_buf_pos = dac_buf_pos + 2;
  }
  else
  {
    current_dac_buf = !current_dac_buf;
    dac_buf_pos=0;
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
void proces_audio(void)
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
    
    int len = udp.read(dac_buf[!current_dac_buf], packetSize);
    
  } 
  }
