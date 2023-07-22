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
