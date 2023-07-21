float getPPMValue(float voltage)
{
  ecValue=(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage)*kValue;
  ecValue25 = ecValue / (1.0+(0.02*(temperature-25.0)));  //temperature compensation
  tdsValue = ecValue25 * 0.5;

  return tdsValue;
}
