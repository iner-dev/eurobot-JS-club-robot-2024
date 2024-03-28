#include <ESP8266WiFi.h>

const char* ssid = "MACing";
const char* password = "password";

IPAddress localIP(192, 168, 4, 1);

void setup() {
  Serial.begin(9600);
  delay(100);

  // Set up AP
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(localIP, localIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
}

void loop() {
  delay(1000);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}
