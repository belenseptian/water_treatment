void sensorCalibration(float bufferPPM, float voltage)
{
  rawECsolution = bufferPPM/tdsFactor;
  rawECsolution = rawECsolution*(1.0+(0.02*(temperature-25.0)));

  KValueTemp = rawECsolution/(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage);  //calibrate in the  buffer solution, such as 707ppm(1413us/cm)@25^c
  if((rawECsolution>0) && (rawECsolution<2000) && (KValueTemp>0.25) && (KValueTemp<4.0))
  {
    Serial.println();
    Serial.print(F(">>>Calibration is successful,K:"));
    Serial.print(KValueTemp);
  }
  else{
    Serial.println("Calibration is failed");
  }      
}

float getPPMValue(float voltage, float temp)
{
  ecValue=(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage)*kValue;
  ecValue25 = ecValue / (1.0+(0.02*(temp-25.0)));  //temperature compensation
  tdsValue = ecValue25 * tdsFactor;

  return tdsValue;
}
