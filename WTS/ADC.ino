void initADC()
{
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);
  
  ADS0.begin();
  ADS1.begin();
  
  Serial.println(ADS0.isConnected());
  Serial.println(ADS1.isConnected());
  
  ADS0.setDataRate(4);  // 7 is fastest, but more noise
  ADS1.setDataRate(4);
  
  idx = 0;
  ADS_request_all();  
}

void ADS_request_all()
{
  if (ADS0.isConnected()) ADS0.requestADC(idx);
  if (ADS1.isConnected()) ADS1.requestADC(idx);
}

bool ADS_read_all()
{
  if (ADS0.isConnected() && ADS0.isBusy()) return true;
  if (ADS1.isConnected() && ADS1.isBusy()) return true;

  if (ADS0.isConnected()) val0[idx] = ADS0.getValue();
  if (ADS1.isConnected()) val1[idx] = ADS1.getValue();
  idx++;
  if (idx < 4)
  {
    ADS_request_all();
    return true;
  }
  idx = 0;
  return false;
}

void ADS_print_all()
{
  uint32_t now = millis();
  // Serial.println(now - lastTime);
  lastTime = now;

  // PRINT ALL VALUES OF ADC0
  for (int i = 0; i < 4; i++)
  {
    Serial.print(val0[i]);
    Serial.print("\t");
  }
  // PRINT ALL VALUES OF ADC1
  for (int i = 0; i < 4; i++)
  {
    Serial.print(val1[i]);
    Serial.print("\t");
  }
  Serial.println();
}

float adcVoltage(int index)
{
  return ADS1.toVoltage(val1[index]);
}
