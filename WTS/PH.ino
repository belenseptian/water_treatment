void PHCalibration(float ph_min, float ph_max, float voltage_ph_min, float voltage_ph_max)
{
  float m = (ph_min-ph_max)/(voltage_ph_min-voltage_ph_max);
  float b = ((m*voltage_ph_max)-ph_max)*-1;

  Serial.print("m : ");
  Serial.println(m);
  Serial.print("b : ");
  Serial.println(b);
}

float getPH(float voltage)
{
  float y = (-5.71*voltage)+17.57;

  return y;
}

float getPHOffset(int adc)
{
  float mapping;
  raw_adc = map(adc, 0, 17555, 0, 210);
  if(raw_adc <= 10)
  {
    mapping = 0;
  }
  else if(raw_adc >= 11 && raw_adc <= 100)
  {
    mapping = (raw_adc - 100)/20;
  }
  else
  {
    mapping = (raw_adc - 100)/20;
  }

  return mapping;
}

float savePHOffset(float offset)
{
  EEPROM.writeFloat(EEPROM_Address, offset);//EEPROM.put(address, param);
  EEPROM.commit();
  float read_data = EEPROM.readFloat(EEPROM_Address);

  return read_data;
}
