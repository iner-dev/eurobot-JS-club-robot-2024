#include "bras.h"

Servo_Bras::Servo_Bras(String index, int pin_bouton_max, int pos_max, int pin_bouton_min, int pos_min): _index(index), _pin_bouton_max(pin_bouton_max), _pos_max(pos_max), _pin_bouton_min(pin_bouton_min), _pos_min(pos_min){
  //
}

void Servo_Bras::init(){
  pinMode(_pin_bouton_max,INPUT_PULLUP);
  pinMode(_pin_bouton_min,INPUT_PULLUP);
}

void Servo_Bras::run(){
  if(!digitalRead(_pin_bouton_max)){
    Serial1.println(_index+String(_pos_max));
  }else if(!digitalRead(_pin_bouton_min)){
    Serial1.println(_index+String(_pos_min));
  }
}
