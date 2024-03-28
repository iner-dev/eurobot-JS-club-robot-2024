#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "ESP-01-AP";
const char* password = "password";

// IP Address and port
IPAddress serverIP(192, 168, 4, 1);
const int serverPort = 8888;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  delay(100);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Connect to server
  client.connect(serverIP, serverPort);
}

void loop() {
  if (client.connected()) {
    if (Serial.available()) {
      char c = Serial.read();
      client.write(c);
    }
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }
}
