#include "bras.h"

Bras bras;

char income;
char val;

void setup(){
  bras.init();
  Serial.begin(9600);
}

void loop(){
  if(Serial.available()>0){
    income = Serial.read();
    val = Serial.parseInt();
    bras.update_serial_input(income, val);
  }
}
