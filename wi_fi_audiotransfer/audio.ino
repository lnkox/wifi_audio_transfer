

int  val = 0;
uint8_t adc_buf[2][buf_size]; // ADC data buffer, double buffered
uint8_t dac_buf[buf_dac_size]; // DAC data buffer
uint8_t tbuf[buf_size]; // temp DAC data buffer
char current_adc_buf = 0;

int play_buf_pos = 0;
int adc_buf_pos = 0;
int send_samples_now;

uint16_t send_pos = 0;
uint16_t splay_pos = 0;
char play_delay = 14;
void sample_isr(void)
{
  digitalWrite(D8, LOW);
  val = (SPI.transfer16( dac_buf[play_buf_pos] << 4 | 0x3000) & 0b0001111111111110) >> 5;
  digitalWrite(D8, HIGH);

  dac_buf[play_buf_pos] = 0;

  play_buf_pos++;
  if (play_buf_pos == buf_dac_size)
  {
    play_buf_pos = 0;
  }

  adc_buf[current_adc_buf][adc_buf_pos] = val;
  adc_buf_pos++;
  if (adc_buf_pos > buf_size)
  {
    adc_buf_pos = 0;
    current_adc_buf = !current_adc_buf;
    send_samples_now = 1;
  }
}

void proces_audio(void)
{
  if (send_samples_now)
  {
    send_samples_now = 0;
    send_audio_packet();
  }

  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    uint8_t pbuf[4];
    udp.read(pbuf, 4);
    udp.read(tbuf, buf_size);
    int resv_pos = (pbuf[1] << 8) + pbuf[0];
    int play_pos = (pbuf[3] << 8) + pbuf[2];
    if (play_pos>0) play_buf_pos=play_pos;
    for (int q = 0; q < buf_size; q++)
    {
      dac_buf[resv_pos++] = tbuf[q];
    }
  }
}
void send_audio_packet(void)
{
  if (play_delay > 0)
  {
    play_delay--;
    splay_pos = 6000;
    if (play_delay == 0)  splay_pos = 1;
  }
  else
  {
    splay_pos = 0;
  }
  udp.beginPacket("255.255.255.255", udp_out);
  udp.write((const uint8_t *)(&send_pos), 2);
  udp.write((const uint8_t *)(&splay_pos), 2);
  udp.write(&adc_buf[!current_adc_buf][0], buf_size);
  udp.endPacket();
  send_pos = send_pos + buf_size;
  if (send_pos == buf_dac_size) send_pos = 0;
}

