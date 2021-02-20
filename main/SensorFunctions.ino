// sensor reading
void ReadMoistureSensors()
{
  uint16_t sum = 0;
  for (byte i = 0; i < soilMoistureSensorCount; i++)
  {
    soilMoisture[i] = MoistureSensorCalibration(analogRead(pinSoilMoisture[i]));
    sum += soilMoisture[i];
  }
  soilMoistureAvg = sum / soilMoistureSensorCount;
}

void RedDHT()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT!");
    return;
  }
  humidity = h;
  temperature = t;
}

void ReadWaterSensor()
{
  waterLevel = WaterSensorCalibration(analogRead(pinWaterSensor));
}

void UpdateSensors()
{
  ReadMoistureSensors();
  RedDHT();
  ReadWaterSensor();
}

void UpdateAnalogSensors()
{
  ReadMoistureSensors();
  ReadWaterSensor();
}

// sensor calibration functions
float MoistureSensorCalibration(uint16_t rawVal)
{
  // analog read --> 0 - 4096
  return rawVal;
  return map(rawVal, 0, 4096, 0, 100);
}

float WaterSensorCalibration(uint16_t rawVal)
{
  return rawVal;
  return map(rawVal, 0, 4096, 0, 100);
}
