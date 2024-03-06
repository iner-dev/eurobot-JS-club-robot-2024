#include <ESP8266WiFi.h>

const char *ssid = "bastien est idiot";
const char *password = "et c'est vrais";

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialisation du mode station (client)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("");
  Serial.println("Point d'Accès WiFi créé !");
  Serial.print("Adresse IP du Point d'Accès: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Vous pouvez ajouter du code supplémentaire ici si nécessaire
}
