#include "partie.h"

Partie::Partie(){
  //
}

void Partie::init(){
  _t0 = millis();
  pinMode(_pin_latch_desarmement,INPUT_PULLUP);
  pinMode(_pin_bouton_start,INPUT_PULLUP);
  pinMode(_pin_bouton_start_pami,INPUT_PULLUP);
}

void Partie::run(){
  button_check();
}

void Partie::button_check(){
  if(digitalRead(_pin_latch_desarmement)){
    if(!digitalRead(_pin_bouton_start)){
      start();
    }
    if(!digitalRead(_pin_bouton_start_pami)){
      start_pami();
    }
  }else{
    Serial1.println("d");
    armed = false;
  }
}

void Partie::start(){
  _t0 = millis();
  _started = true;
  _PAMI_start_sent = false;
  Serial1.println("a");
  armed = true;
}

void Partie::start_pami(){
  Serial1.println("S");
}

void Partie::time_check(){
  if(_started){
    _time = millis()-_t0;
    //Serial.println(_time);
    if((_PAMI_start_sent == false) && (_time >= _demarage_PAMI) && (_time < (_demarage_PAMI+500))){
      _PAMI_start_sent = true;
      Serial1.println("S");
    }
    if(_fin <= _time){
      _started = false;
      Serial1.println("d");
      armed = false;
    }
  }
}
