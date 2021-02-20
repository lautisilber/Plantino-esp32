// pump control
void PumpControl()
{
  byte desiredState = 0; // 0 -> great; 1 -> too humid; 2-> too dry
  if (soilMoistureAvg > maxHumidity)
    desiredState = 1;
  else if (soilMoistureAvg < minHumidity)
    desiredState = 2;

  if (desiredState == 0) // check for overfloods or dry-ups
  {
    if (waterLevel >= maxWaterValue || waterLevel <= minWaterValue)
      PumpSetState(0);
  }
  else if (desiredState == 1) // try to drain
  {    
    if (waterLevel <= minWaterValue)
      PumpSetState(0);
    else
      PumpSetState(2);
  }
  else if (desiredState == 2) // try to fill
  {
    if (waterLevel >= maxWaterValue)
      PumpSetState(0);
    else
      PumpSetState(1);
  }
}

void PumpSetState(byte state)
{
  // 0 -> closed, closed
  // 1 -> open, closed
  // 2 -> closed, open
  // 3 -> open, open | (we never want to do this so it's not implemented)
  if (state == 0)
  {
    if (pumpState1)
    {
      digitalWrite(pinPump1, LOW);
      pumpState1 = false;
    }
    if (pumpState2)
    {
      digitalWrite(pinPump2, LOW);
      pumpState2 = false;
    }
  }
  else if (state == 1)
  {
    if (!pumpState1)
    {
      digitalWrite(pinPump1, HIGH);
      pumpState1 = true;
    }
    if (pumpState2)
    {
      digitalWrite(pinPump2, LOW);
      pumpState2 = false;
    }
  }
  else if (state == 2)
  {
    if (pumpState1)
    {
      digitalWrite(pinPump1, LOW);
      pumpState1 = false;
    }
    if (!pumpState2)
    {
      digitalWrite(pinPump2, HIGH);
      pumpState2 = true;
    }
  }  
  else
    Serial.println("ERROR AT FUNC 'PumpSetSave'. FIX THIS!!!");
}
