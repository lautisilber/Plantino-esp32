// helper functions
bool DoTick(uint32_t now, uint32_t lastTime, uint32_t interval)
{
  if (now > lastTime && now - lastTime >= interval)
  // this behaves normally
    return true;
  else if (now < lastTime && now + pow(2, sizeof(now)*8)-1 - lastTime >= interval)
  // when there is an overflow in millis(), the interval is millis() + ("max millis possible" - lastTime)
    return true;
  return false;
}

void InfoPin(bool state)
{
  digitalWrite(pinInfo, state);
}

byte PumpToInt(bool state)
{
  return (state ? 1 : 0);
}

String PumpToStr(bool state)
{
  return (state ? "on" : "off");
}

bool UseConfigCredentials(fs::FS &fs, const char * path)
{
  File file = fs.open(path);
  if(!file){
      return false;
  }

  StaticJsonDocument<100> doc;
  deserializeJson(doc, file);

  const char* sd_ssid = doc["ssid"];
  const char* sd_password = doc["password"];
  Serial.print(sd_ssid);
  WiFi.begin(sd_ssid, sd_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  return true;
}
