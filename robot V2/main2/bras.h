#ifndef bras_h
#define bras_h

#include <Servo.h>
#include <Arduino.h>

class Bras
{
public:
  Bras();
  void init();
  void run();
  void update_serial_input(char index, int val);
private:
  bool _armed = false;;
  static const int _eqi_P = 180;
  static const int _pin_servo_Chassie = 2;
  static const int _pin_servo_P1 = 3;
  static const int _pin_servo_P2 = 4;
  Servo _servoChassie;
  Servo _servoP1;
  Servo _servoP2;
  byte _chassisMin;
  byte _chassisMax;
  byte _pinceMin;
  byte _pinceMax;
  unsigned long _tp;
  int valp;
  int dval;
  bool fermeturePince;

  enum EtatPince
  {
    Ouverte=0,
    Fermee=1,
    Ouverture=2,
    Fermeture=3
  } etatPince;

  void actionnerPince(int val);
};


#endif
