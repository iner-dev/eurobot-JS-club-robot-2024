//intégration
#include "matrices_robot.h"
#include "moteurs.h"
#include "bras.h"
#include "ESP.h"

Bras bras;
Motorisation motor;
Matrices_robot matt;



//definition des variables
char serial_income; //  arivée de données venant du port serie le port serie
int val;
unsigned long animation_start = millis();



//////////////////////////////////////////////////////////////      fonctions d'executions       ////////////////////////////////////////////////////////////////

void setup() {
  // ouverture du port serie pour la comunication et le debug
  Serial.begin(115200);
  Serial1.begin(38400);
  Serial2.begin(115200);
  matt.init();
  bras.init();
  motor.init();
}

//boucle principale
void loop() {
  if(Serial1.available()){
    String str = Serial1.readStringUntil('\n');
    serial_income = str[0];
    val = str.substring(1).toInt();
    esp_update_serial_input(serial_income, val);
    bras.update_serial_input(serial_income, val);
    motor.update_serial_input(serial_income, val);
    matt.update_serial_input(serial_income, val);
  }
  matt.run();
  bras.run();
}
