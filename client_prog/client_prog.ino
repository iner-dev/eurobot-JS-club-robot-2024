#include <ESP8266WiFi.h>

const char* ssid = "ESP01_Server";
const char* password = "password";
const char* serverIP = "192.168.4.1"; // Change to the IP address of the server ESP01
const int serverPort = 80;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Server IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Establish connection with the server
  WiFiClient client;
  if (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection failed");
  }else{
    client.print("GET / HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(serverIP);
    client.print("\r\n");
    client.print("Connection: close\r\n\r\n");
    if (client.available()) {
      char c = client.read();
      Serial.println(c); // Output received bytes to serial port
    }
    client.stop();
    delay(10);
  }
}
