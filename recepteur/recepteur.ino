#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "YourNetworkName"; // Change this to your WiFi network's SSID
const char* password = "YourPassword"; // Change this to your WiFi network's password

WiFiUDP udp;
String msg;

void msgsend(IPAddress ip,const int udpPort,String msg){
  udp.beginPacket(ip, udpPort);
  udp.print(msg);
  udp.endPacket();
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  
  Serial.println("/WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("/WiFi connected");
  Serial.println("/IP address: ");
  Serial.println(WiFi.localIP());

  udp.begin(1234);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[255];
    udp.read(packetBuffer, packetSize); // Lecture des données dans le tampon
    Serial.print("Message reçu: ");
    Serial.println(packetBuffer); // Affichage du message reçu
  }
  delay(1);
}
