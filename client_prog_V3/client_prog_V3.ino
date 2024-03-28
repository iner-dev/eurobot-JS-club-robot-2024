#include <SPI.h>
#include <WiFi101.h>
#include <ESP8266WiFi.h>

const char* ssid = "ESP-01-AP";
const char* password = "password";
const int port = 8888; // Choose any available port number
typedef uint32_t uint32;


WiFiServer server(port);

void setup() {
  Serial.begin(9600);
  delay(10);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");
}

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

void broadcast(const String& message, WiFiClient& sender) {
  IPAddress senderIP = sender.remoteIP();
  IPAddress localIP = WiFi.localIP();
  IPAddress subnetMask = WiFi.subnetMask();

  if (subnetMask[0] == 255 && subnetMask[1] == 255 && subnetMask[2] == 255 && subnetMask[3] == 0) {
    for (uint8_t i = 1; i < 255; i++) {
      IPAddress broadcastIP(localIP[0], localIP[1], localIP[2], i);
      if (broadcastIP != senderIP) {
        WiFiClient client;
        if (client.connect(broadcastIP, port)) {
          client.println(message);
          client.stop();
        }
      }
    }
  }
}
