

int  val = 0;
uint16_t adc_buf[2][buf_size]; // ADC data buffer, double buffered
uint16_t dac_buf[DAC_BUF_CNT][buf_size]; // DAC data buffer, double buffered
uint8_t tbuf[buf_dac_size];
char current_adc_buf = 0;
char current_dac_buf = 0;
char play_dac_buf = 0;
boolean play_en = false;
int adc_buf_pos = 0;
int dac_buf_pos = 0;
int send_samples_now;
int buf_cnt = 0;
boolean buf_live[DAC_BUF_CNT];

float FK = 0.15;
int acc_xf;

void sample_isr(void)
{


  digitalWrite(D8, LOW);
  val = SPI.transfer16( dac_buf[play_dac_buf][dac_buf_pos] | 0x3000) & 0x0fff;
  digitalWrite(D8, HIGH);

  if (play_en)
  {
    dac_buf_pos++;

    if (dac_buf_pos == buf_size)
    {
      play_dac_buf++;
      if (play_dac_buf == DAC_BUF_CNT)
      {
        play_dac_buf = 0;
      }
      dac_buf_pos = 0;
      buf_cnt--;
    }
  }
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
    send_audio_packet();
  }
if (buf_cnt<1) {play_en=false;}
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    int buf16pos = 0;
    int len = udp.read(tbuf, packetSize);
    for (int q = 0; q < packetSize; q = q + 2)
    {
      dac_buf[current_dac_buf][buf16pos++] = (tbuf[q + 1] << 8) + tbuf[q];
    }
    current_dac_buf++;
    if (current_dac_buf == DAC_BUF_CNT)
    {
      current_dac_buf = 1;
    }

    buf_cnt++;
    if (buf_cnt == DAC_BUF_CNT - 1)
    {
      play_en = true;
    }

  }
}
void send_audio_packet(void)
{
  udp.beginPacket("255.255.255.255", udp_out);
  udp.write((const uint8_t *)(&adc_buf[!current_adc_buf][0]), buf_size * 2);
  udp.endPacket();
}

