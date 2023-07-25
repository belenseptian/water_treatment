/* Libraries */
#include "ADS1X15.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Fuzzy.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* Constants */
#define temperature 30.19
#define kValue 0.81
#define tdsFactor 0.5
#define EEPROM_SIZE 12
#define EEPROM_Address 1
#define ONE_WIRE_BUS 15
#define pump 32
#define pump_low 5000
#define pump_high 10000

/* ADC */
int16_t val0[4] = { 0, 0, 0, 0 };
int16_t val1[4] = { 0, 0, 0, 0 };
int idx = 0;
uint32_t lastTime = 0;

/* WiFi */
const char *ssid = "TKJ OK";         // Enter your WiFi name
const char *password = "12345@TKJOK";  // Enter WiFi password

/* MQTT Broker */
const char *mqtt_broker = "o2cd700d.ala.us-east-1.emqxsl.com";  // broker address
const char *topic = "room/lamp";                                // define topic
const char *mqtt_username = "alam";                             // username for authentication
const char *mqtt_password = "Qwert1234";                        // password for authentication
const int mqtt_port = 8883;                                     // port of MQTT over TLS/SSL
// load DigiCert Global Root CA ca_cert
const char *ca_cert =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
  "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
  "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
  "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
  "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
  "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
  "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
  "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
  "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
  "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
  "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
  "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
  "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
  "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
  "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4="
  "-----END CERTIFICATE-----\n";

/* JSON */
char JSONmessageBuffer[200];

/* TDS (PPM) */
float ecValue, ecValue25, tdsValue, KValueTemp, rawECsolution;

/* PH */
int raw_adc;

/* EEPROM */
float read_data;

/* Millis */
long now = millis(), lastMeasure = 0;
long now_ = millis(), lastMeasure_ = 0;
long now__ = millis(), lastMeasure__ = 0;

/* Timer Setup */
hw_timer_t * disp_timer = NULL;

/* Fuzzy */
// FuzzyInput PH Sensor
FuzzySet *phfair = new FuzzySet(5.5, 6, 6, 7);
FuzzySet *phgood = new FuzzySet(6.5, 7, 8, 8.5);
FuzzySet *phbad = new FuzzySet(8, 8.5, 8.5, 10.5);
// FuzzyInput TDS
FuzzySet *excellent = new FuzzySet(0, 110, 110, 120);
FuzzySet *good = new FuzzySet(110, 120, 210, 220);
FuzzySet *fair = new FuzzySet(210, 220, 220, 320);
// FuzzyOutput Water Quality
FuzzySet *bad = new FuzzySet(0, 30, 30, 33);
FuzzySet *medium = new FuzzySet(30, 33, 60, 66);
FuzzySet *wqgood = new FuzzySet(60, 66, 66, 99);

int pump_status = 0;

/* Classes */
ADS1115 ADS0(0x49);
ADS1115 ADS1(0x48);
// init secure wifi client
WiFiClientSecure espClient;
// use wifi client to init mqtt client
PubSubClient client(espClient);
// init JSON library
StaticJsonBuffer<200> jsonBuffer;
JsonObject &root = jsonBuffer.createObject();
Fuzzy *fuzzy = new Fuzzy();
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Init EEPROM
  EEPROM.begin(EEPROM_SIZE);
  //Init WiFi
  initWifi();
  initMQTT();
  initADC();
  initFuzzy();
  sensors.begin();
  pinMode(pump, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  while (ADS_read_all());
  now = millis();
  if (now - lastMeasure > 1000) {
    lastMeasure = now;
    
//    PHCalibration(7, 9, 1.85, 1.5);
//    Serial.println(getPPMValue(adcVoltage(3), temperature));
//    Serial.println(getPH(adcVoltage(2)));
    
//    EEPROM.writeFloat(EEPROM_Address, getPHOffset(val1[1]));//EEPROM.put(address, param);
//    EEPROM.commit();
//    read_data = EEPROM.readFloat(EEPROM_Address);
//    Serial.println(read_data);

//    Serial.println(getTemp());

//    Serial.println(fuzzyOut(getPPMValue(adcVoltage(3), temperature), getPH(adcVoltage(2))-savePHOffset(getPHOffset(val1[1]))));
    
    Serial.print(F("PPM = "));
    Serial.println(getPPMValue(adcVoltage(3), getTemp()));
    float PPMValue = getPPMValue(adcVoltage(3), getTemp()); 
    client.publish("sensor/tds", String(PPMValue).c_str());

    Serial.print(F("PH = "));
    Serial.println(getPH(adcVoltage(2))-savePHOffset(getPHOffset(val1[1])));
    float phValue = getPH(adcVoltage(2))-savePHOffset(getPHOffset(val1[1])); 
    client.publish("sensor/ph", String(phValue).c_str());

    Serial.print(F("TEMP = "));
    Serial.println(getTemp());
    float tmpValue = getTemp(); 
    client.publish("sensor/temperature", String(tmpValue).c_str());

    Serial.print(F("QUALITY = "));
    Serial.println(fuzzyOut(getPPMValue(adcVoltage(3), temperature), getPH(adcVoltage(2))-savePHOffset(getPHOffset(val1[1]))));
    float fuzzyValue = fuzzyOut(getPPMValue(adcVoltage(3), temperature), getPH(adcVoltage(2))-savePHOffset(getPHOffset(val1[1]))); 
    client.publish("sensor/quality", String(fuzzyValue).c_str());

    root["tds"] = getPPMValue(adcVoltage(3), getTemp());
    root["ph"]  = getPH(adcVoltage(2))-savePHOffset(getPHOffset(val1[1]));
    root["temperature"]  = getTemp();
    root["quality"]  = fuzzyOut(getPPMValue(adcVoltage(3), temperature), getPH(adcVoltage(2))-savePHOffset(getPHOffset(val1[1])));
    root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    client.publish("sensor/db", JSONmessageBuffer);
  }
  //request all the adc values
  ADS_request_all();

  now_ = millis();
  if (now_ - lastMeasure_ > 5000) {
    lastMeasure_ = now_;
    digitalWrite(pump,LOW);
  }  
  
  now__ = millis();
  if (now__ - lastMeasure__ > 10000) {
    lastMeasure__ = now__;
    digitalWrite(pump,HIGH);
  }
}
