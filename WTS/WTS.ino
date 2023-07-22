/* Libraries */
#include "ADS1X15.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

/* Constants */
#define temperature 30.19
#define kValue 0.81
#define tdsFactor 0.5

/* ADC */
int16_t val0[4] = { 0, 0, 0, 0 };
int16_t val1[4] = { 0, 0, 0, 0 };
int idx = 0;
uint32_t lastTime = 0;

/* TDS (PPM) */
float ecValue,ecValue25,tdsValue,KValueTemp,rawECsolution;

/* Millis */
long now = millis(),lastMeasure = 0;

/* Classes */
ADS1115 ADS0(0x49);
ADS1115 ADS1(0x48);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initADC();
}

void loop() {
  while (ADS_read_all());
  now = millis();
  if (now - lastMeasure > 1000) {
    lastMeasure = now;
    //  PHCalibration(7, 9, 1.85, 1.5);
    Serial.println(getPPMValue(adcVoltage(3), temperature));  
    Serial.println(getPH(adcVoltage(2)));
  }
  //request all the adc values
  ADS_request_all();
}
