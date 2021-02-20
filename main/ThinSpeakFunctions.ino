void UploadThingSpeak()
{
  ThingSpeak.setField(1, soilMoistureAvg);
  ThingSpeak.setField(2, temperature);
  ThingSpeak.setField(3, humidity);
  ThingSpeak.setField(4, waterLevel);
  ThingSpeak.setField(5, PumpToInt(pumpState1));
  ThingSpeak.setField(6, PumpToInt(pumpState2));
  ThingSpeak.setField(7, GetErrorCode());

  ThingSpeak.writeFields(channelID, channelApiKey);
}

byte GetErrorCode()
{
  if (!sdStatus && !spiffsStatus)
    return 3;
  else if (!spiffsStatus)
    return 2;
  else if (!sdStatus)
    return 1;
  return 0;
}
