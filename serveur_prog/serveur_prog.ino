#include <ESP8266WiFi.h>

const char* ssid = "ESP01_Server";
const char* password = "password";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set up ESP01 as an Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  client.flush();

  // Send a response to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<body>");
  client.println("<h1>Hello from ESP8266!</h1>");
  client.println("</body>");
  client.println("</html>");
}
