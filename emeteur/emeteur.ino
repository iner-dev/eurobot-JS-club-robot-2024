#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "YourNetworkName";     // Change this to the SSID you want for the emitter's WiFi network
const char* password = "YourPassword";    // Change this to the password you want for the emitter's WiFi network
const int udpPort = 1234;                 // Port for UDP communication

WiFiUDP udp;
String msg;


void setup() {
  Serial.begin(9600);
  delay(1000);
  // Set ESP8266 to access point mode
  WiFi.mode(WIFI_AP);
  
  // Create WiFi network
  WiFi.softAP(ssid, password);

  Serial.println("WiFi access point created");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Send UDP packets to a specific IP address and port
  if(Serial.available()){
    msg = Serial.read();
    IPAddress ip (192, 168, 4, 2);     
    udp.beginPacket(ip, udpPort);
    udp.println(msg);
    udp.endPacket();
    Serial.print("sended to ");
    Serial.println(ip);
  }
}
