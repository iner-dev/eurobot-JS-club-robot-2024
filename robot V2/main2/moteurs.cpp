#include "moteurs.h"

#include <Arduino.h>

const int Motorisation::_reset_pin = 10; 

void Motorisation::init(){
  Wire.begin();
  broadcast_motors_speed(0);
  pinMode(_reset_pin,OUTPUT);
  digitalWrite(_reset_pin,HIGH);
  _armed = false;
  _r1 = 0;
  _r2 = 0;
  _r3 = 0;
}

void Motorisation::motor_send(int addr,int val){
  Wire.beginTransmission(addr); 
/*  Serial.print(addr);
  Serial.print(" ");
  Serial.println(val);
 */ 
  Wire.write(val >> 8);      
  Wire.write(val & 0xFF); 
  Wire.write(0);    
  Wire.endTransmission();
}

void Motorisation::update_motors_speed(){
  motor_send(0x01,_r1);
  motor_send(0x02,_r2);
  motor_send(0x03,_r3);
}

void Motorisation::broadcast_motors_speed(int val){
  motor_send(0x01,val);
  motor_send(0x02,val);
  motor_send(0x03,val);
}

void Motorisation::update_serial_input(char index,int val){
  if(_armed){
    if(index =='1'){
      _r1 = val;
    }else if(index =='2'){
      _r2 = val;
    }else if(index =='3'){
      _r3 = val;
    }else if(index =='u'){
      update_motors_speed();
    }else if(index =='R'){
      digitalWrite(_reset_pin,LOW);
      delay(200);
      digitalWrite(_reset_pin,HIGH);
    }
  }
  if(index =='d'){ 
    _armed = false;
//    Serial.println("Disarmed");
    broadcast_motors_speed(0);
  }else if(index =='a'){
    _armed = true;
//    Serial.println("Armed");
    update_motors_speed();
  }
}
