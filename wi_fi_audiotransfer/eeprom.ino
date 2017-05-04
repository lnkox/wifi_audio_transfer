void readSet() //Читання налаштувань з енергонезалежної памяті в оперативну
{
  Serial.println("read set");
  int i;
  byte eeprom_data_tmp[sizeof(eeprom_data)];
  EEPROM.begin(sizeof(eeprom_data));
  for (i = 0; i < sizeof(eeprom_data); i++)
  {
    eeprom_data_tmp[i] = EEPROM.read(i);
  }
  memcpy(&eeprom_data, eeprom_data_tmp,  sizeof(eeprom_data));
}

void writeSet() //Запис налаштувань з оперативної памяті в енергонезалежну
{
  Serial.println("write set");
  int i;
  byte eeprom_data_tmp[sizeof(eeprom_data)];
  EEPROM.begin(sizeof(eeprom_data));
  memcpy(eeprom_data_tmp, &eeprom_data, sizeof(eeprom_data));
  for (i = 0; i < sizeof(eeprom_data); i++)
  {
    EEPROM.write(i, eeprom_data_tmp[i]);
  }
  EEPROM.commit();
}
