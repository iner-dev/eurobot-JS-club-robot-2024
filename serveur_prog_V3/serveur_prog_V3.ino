#include <ESP8266WiFi.h>

const char* ssid = "ESP-01-AP";
const char* password = "password";
const int port = 8888; // Choose any available port number

WiFiServer server(port);

IPAddress localIP(192, 168, 4, 1);

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
#include <ESP8266WiFi.h>


void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    while (client.connected()) {
      if (client.available() > 0) {
        // Read data from the client
        String data = client.readStringUntil('\n');
        Serial.println(data);
      }
    }
    client.stop();
  }
  if(Serial.available()){
    broadcast(Serial.readStringUntil('\n'), client);
  }
}

void broadcast(String message, WiFiClient sender) {
  for (int i = 0; i < 4; i++) {
    if (i + 1 != sender.remoteIP()[3]) { // Exclude the sender
      WiFiClient client;
      if (client.connect(WiFi.gatewayIP(), port)) {
        client.println(message);
        Serial.print("Sent to client ");
        Serial.println(i + 1);
        client.stop();
      } else {
        Serial.println("Connection failed");
      }
    }
  }
}
