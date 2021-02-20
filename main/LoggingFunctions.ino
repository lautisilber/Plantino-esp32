// logging
void UpdateJson()
{
  jsonDoc[jsonTitles[0]] = prettyDate;
  for (byte i = 0; i < soilMoistureSensorCount; i++)
    soilMoistureJson[i] = soilMoisture[i];
  jsonDoc[jsonTitles[2]] = soilMoistureAvg;
  jsonDoc[jsonTitles[3]] = humidity;
  jsonDoc[jsonTitles[4]] = temperature;
  jsonDoc[jsonTitles[5]] = waterLevel;
  jsonDoc[jsonTitles[6]] = pumpState1;
  jsonDoc[jsonTitles[7]] = pumpState2;
}

void LogReadings()
{
  UpdateTime();
  UpdateJson();
  LogJson(SD, fileName, jsonDoc);
}
