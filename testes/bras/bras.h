#ifndef bras_h
#define bras_h

#include <Servo.h>

class Bras
{
public:
  Bras();
  void init();
  void update_serial_input(char index, int val);
private:
  bool _armed = false;;
  static const int _eqi_P = 120;
  static const int _pin_servo_Chassie = 2;
  static const int _pin_servo_P1 = 3;
  static const int _pin_servo_P2 = 4;
  Servo _servoChassie;
  Servo _servoP1;
  Servo _servoP2;
};


#endif
