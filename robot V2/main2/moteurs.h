#ifndef moteurs
#define moteurs
#include <Wire.h>

class Motorisation
{
public:
  motorisation();
  void init();
  void update_serial_input(char index,int val);
private:
  void update_motors_speed();
  void broadcast_motors_speed(int val);
  static const int _reset_pin; 
  bool _armed;
  float _r1;
  float _r2;
  float _r3;
  void motor_send(int addr,int val);
};

#endif
