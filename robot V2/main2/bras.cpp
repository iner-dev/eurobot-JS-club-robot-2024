#include "bras.h"

Bras::Bras(){
  //
}

void Bras::init(){
  _chassisMin = 119;
  _chassisMax = 25;
  _pinceMin = 50;
  _pinceMax = 170;
  valp = 0;
  dval = 10;
  fermeturePince = false;
  _tp=0;
  if(_armed){
    _servoChassie.attach(_pin_servo_Chassie);
//    _servoP1.attach(_pin_servo_P1);
//    _servoP2.attach(_pin_servo_P2);
  }else{
    _servoChassie.detach();
  }
  _servoP1.detach();
  _servoP2.detach();
  etatPince = Ouverte;
 /* _servoChassie.write(30);
  _servoP1.write(20);
  _servoP2.write(100);*/
}

void Bras::update_serial_input(char index, int val){
  if((index == 'G') && _armed){ 
//    Serial.print("Pince : ");
//    Serial.println(map(val, 0, 255, 50, 160));
    if (val<250)
    {
      _servoP1.attach(_pin_servo_P1);
      _servoP2.attach(_pin_servo_P2);
      actionnerPince(val);
      etatPince = Ouverture;
      _tp=millis();
    }
    else if (etatPince == Ouverte)
    {
      _servoP1.attach(_pin_servo_P1);
      _servoP2.attach(_pin_servo_P2);
      valp=0;
      dval=80;
      etatPince = Fermeture;
      _tp=millis();
    }
    
  }else if(index == 'b'){
    Serial.print("Bras  : ");
    Serial.println(map(val, 0, 255, _chassisMin, _chassisMax));
    _servoChassie.write(map(val, 0, 255, _chassisMin, _chassisMax));
  }else if(index == 'd'){
    _armed = false;
    _servoChassie.detach();
    _servoP1.detach();
    _servoP2.detach();
  }else if(index == 'a'){
    _armed = true;
    _servoChassie.attach(_pin_servo_Chassie);
  }
}

void Bras::actionnerPince(int val)
{
  _servoP1.write(map(val, 0, 255, _pinceMin, _pinceMax));
  _servoP2.write(180-map(val, 0, 255, _pinceMin, _pinceMax));
}

void Bras::run()
{
  unsigned long t = millis();
  if ((etatPince == Ouverture) && ((t - _tp) > 200))
  {
    _servoP1.detach();
    _servoP2.detach();
    etatPince=Ouverte;
  }
  if ((etatPince == Fermeture) && ((t - _tp) > 50))
  {
    valp += dval;
    actionnerPince(valp+dval);
    if (dval>0)
      dval=-2;
    else
      dval=40;
    _tp=t;
    if (valp>=255)
    {
      actionnerPince(255);
      etatPince = Fermee;
    }
  }
}
