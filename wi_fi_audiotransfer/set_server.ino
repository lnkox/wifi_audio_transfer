void handleRoot() { // Генерація та відправка сторінки налаштувань
  String strm = "";
  strm += " <html><head>";
  strm += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>";
  strm += "<meta http-equiv='Content-Language' content='uk'>";
  strm += "<title>WI-FI audio transfer</title>";
  strm += "</head>";
  strm += "<body>";
  strm += "<h1 align='center'>WI-FI audio transfer</h1>";
  strm += "<form method='POST' action='/set'>";
  strm += "  <table border='0' width='95%'>";
  strm += "    <tr>";
  strm += "      <td width='155'>SSID(Назва мережі): </td>";
  strm += "      <td width='572'><input type='text' name='ssid' id='ssid' value='" + String(eeprom_data.eessid) + "' size='35'></td>";
  strm += "    </tr>";
  strm += "    <tr>";
  strm += "      <td width='155'>Пароль: </td>";
  strm += "      <td width='572'><input type='text' name='pass' value='" + String(eeprom_data.eepass) + "' size='35'></td>";
  strm += "    </tr>";
  strm += "    <tr>";
  strm += "      <td width='727' colspan='2' align='center'>";
  strm += "      <input type='submit' value='Зберегти' name='B1'></td>";
  strm += "    </tr>";
  strm += "  </table>";
  strm += "</form>";
  strm += "</body>";
  strm += "</html>";
  server.send(200, "text/html", strm);
}
void handleSet() // Збереження налаштувань від користувача та генерація і відправка сторінки зі збереженими налаштуваннями
{
  String strm = "";
  server.arg("ssid").toCharArray(eeprom_data.eessid, sizeof(eeprom_data.eessid));
  server.arg("pass").toCharArray(eeprom_data.eepass, sizeof(eeprom_data.eepass));
  writeSet();
  strm += " <html><head>";
  strm += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>";
  strm += "<meta http-equiv='Content-Language' content='uk'>";
  strm += "<title>WI-FI audio transfer</title>";
  strm += "</head>";
  strm += "<body>";
  strm += "<h1 align='center'>WI-FI audio transfer</h1>";
  strm += "<h3 align='center'>Налаштування збережено</h1>";
  strm += "<br>SSID(Назва мережі):" + String(eeprom_data.eessid);
  strm += "<br>Пароль:" + String(eeprom_data.eepass);
  strm += "</body>";
  strm += "</html>";
  server.send(200, "text/html", strm);
}
