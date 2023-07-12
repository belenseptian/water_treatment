#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#define led_pin 2

// WiFi
const char *ssid = "robot"; // Enter your WiFi name
const char *password = "umdprobot";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "f111ecc2.ala.us-east-1.emqxsl.com";// broker address
const char *topic = "room/lamp"; // define topic 
const char *mqtt_username = "conn_test"; // username for authentication
const char *mqtt_password = "12345";// password for authentication
const int mqtt_port = 8883;// port of MQTT over TLS/SSL

// load DigiCert Global Root CA ca_cert
const char* ca_cert= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=" \
"-----END CERTIFICATE-----\n";

long now = millis();
long lastMeasure = 0;

// init secure wifi client
WiFiClientSecure espClient;
// use wifi client to init mqtt client
PubSubClient client(espClient); 

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String messageTemp, topic_;
  topic_ = topic;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
    messageTemp += (char) payload[i];
  }
  Serial.println();
  Serial.println("-----------------------");

  if(topic_ == "room/lamp")
  {
    if(messageTemp == "on") digitalWrite(led_pin, HIGH);
    else digitalWrite(led_pin, LOW);
  }
}

void reconnect()
{
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } 
    else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println("Retrying in 5 seconds.");
      delay(5000);
    }
  }  
}

void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  // set root ca cert
  espClient.setCACert(ca_cert);
  // connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  reconnect();
  // subscribe
  client.subscribe(topic); // subscribe from the topic
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - lastMeasure > 1000) {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    int humidity = random(300);

    // Publishes Temperature and Humidity values
    client.publish("room/temperature", String(humidity).c_str());
    
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }
}
