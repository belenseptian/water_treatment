void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String messageTemp, topic_;
    topic_ = topic;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        messageTemp += (char)payload[i];
    }
    Serial.println();
    Serial.println("-----------------------");

    if (topic_ == "room/lamp")
    {
        // Add your logic here for processing the message for the topic "room/lamp"
        // For example, you can uncomment the code below and customize it as needed.
        /*
        if (messageTemp == "on")
            digitalWrite(led_pin, HIGH);
        else
            digitalWrite(led_pin, LOW);
        */
    }
}

void reconnect() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println("Retrying in 5 seconds.");
      delay(5000);
    }
  }
}

void initWifi()
{
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");  
}

void initMQTT()
{
  // set root ca cert
  espClient.setCACert(ca_cert);
  // connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  reconnect();
  
  // subscribe
  client.subscribe(topic);  // subscribe from the topic  
}
