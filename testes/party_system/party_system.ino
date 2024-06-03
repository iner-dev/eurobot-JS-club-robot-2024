#include "partie.h"

void setup() {
  pinMode(47,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  Serial.println(digitalRead(47));
}
