/* Libraries */
#include "ADS1X15.h"

/* ADC */
int16_t val0[4] = { 0, 0, 0, 0 };
int16_t val1[4] = { 0, 0, 0, 0 };
int     idx = 0;
uint32_t lastTime = 0;

/* Classes */
ADS1115 ADS0(0x49);
ADS1115 ADS1(0x48);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initADC();
}

void loop() {
  // put your main code here, to run repeatedly:
  // wait until all the adc values are read
  while (ADS_read_all());

  //read adc values here...
  Serial.println(ADS1.toVoltage(val1[3]));
  
  delay(1000);
  //request all the adc values
  ADS_request_all();

}
