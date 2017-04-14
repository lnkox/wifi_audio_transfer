

int  val = 0;
uint16_t adc_buf[2][buf_size]; // ADC data buffer, double buffered
uint8_t dac_buf[2][buf_dac_size]; // DAC data buffer, double buffered
uint16_t filtered[buf_size];
char current_adc_buf = 0;
char current_dac_buf = 0;
int adc_buf_pos = 0;
int dac_buf_pos = 0;
int send_samples_now;
boolean buf_live[2];

float FK = 0.15;
int acc_xf;
void sample_isr(void)
{
    uint16_t dac_val = (dac_buf[current_dac_buf][dac_buf_pos + 1] << 8) + dac_buf[current_dac_buf][dac_buf_pos ];
    if (dac_buf_pos < buf_dac_size)
    {
      dac_buf_pos = dac_buf_pos + 2;
    }
    else
    {
      buf_live[current_dac_buf] = false;
      if (buf_live[!current_dac_buf] == true)
      {
        current_dac_buf = !current_dac_buf;
        dac_buf_pos = 0;
      }
      else
      {
        dac_val = 0;
      }
    }
 // uint16_t dac_val = 0;
  digitalWrite(D8, LOW);
  val = SPI.transfer16(dac_val | 0x3000) & 0x0fff;
  digitalWrite(D8, HIGH);


  acc_xf = (1 - FK) * acc_xf + FK * val;
  adc_buf[current_adc_buf][adc_buf_pos] = acc_xf;
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
    send_audio_packet();
  }

  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    int len = udp.read(dac_buf[!current_dac_buf], packetSize);
    if (buf_live[0]==false && buf_live[1]==false) delay(14);
    buf_live[!current_dac_buf] = true;
  }
}
void send_audio_packet(void)
{
  udp.beginPacket("255.255.255.255", udp_out);
  udp.write((const uint8_t *)(&adc_buf[!current_adc_buf][0]), buf_size * 2);
  udp.endPacket();
}

