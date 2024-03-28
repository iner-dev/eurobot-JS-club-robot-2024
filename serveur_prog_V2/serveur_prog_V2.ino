#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "ESP-01-AP";
const char* password = "password";

// IP Address and port
IPAddress localIP(192, 168, 4, 1);
const int localPort = 8888;

WiFiServer server(localPort);

void setup() {
  Serial.begin(9600);
  delay(100);

  // Set up AP
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(localIP, localIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  // Start server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      if (Serial.available()) {
        char c = Serial.read();
        client.write(c);
      }
    }
    client.stop();
  }
}
