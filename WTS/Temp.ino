float getTemp()
{
  sensors.requestTemperatures(); 
  
//  Serial.print("Celsius temperature: ");
//  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
//  Serial.print(sensors.getTempCByIndex(0)); 
//  Serial.print(" - Fahrenheit temperature: ");
//  Serial.println(sensors.getTempFByIndex(0));

  return sensors.getTempCByIndex(0);
}
