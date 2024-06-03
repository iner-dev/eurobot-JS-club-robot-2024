#include "bras.h"

Bras::Bras(){
  //
}

void Bras::init(){
  if(_armed){
    _servoChassie.attach(_pin_servo_Chassie);
    _servoP1.attach(_pin_servo_P1);
    _servoP2.attach(_pin_servo_P2);
  }else{
    _servoChassie.detach();
    _servoP1.detach();
    _servoP2.detach();
  }
  _servoChassie.write(30);
  _servoP1.write(20);
  _servoP2.write(100);
}

void Bras::update_serial_input(char index, int val){
  if(index == 'G'){ 
    _servoP1.write(val);
    _servoP2.write(_eqi_P - val);
  }else if(index == 'b'){
    _servoChassie.write(val);
  }else if(index == 'd'){
    _armed = false;
    _servoChassie.detach();
    _servoP1.detach();
    _servoP2.detach();
  }else if(index == 'a'){
    _armed = true;
    _servoChassie.attach(_pin_servo_Chassie);
    _servoP1.attach(_pin_servo_P1);
    _servoP2.attach(_pin_servo_P2);
  }
}
