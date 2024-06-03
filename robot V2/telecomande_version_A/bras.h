#ifndef Bras_h
#define Bras_h
#include <Arduino.h>

class Servo_Bras{
public:
  Servo_Bras(String index, int pin_bouton_max, int pos_max, int pin_bouton_min, int pos_min);
  void init();
  void run();
private:
  String _index;
  int _pin_bouton_max;
  int _pos_max;
  int _pin_bouton_min;
  int _pos_min;
};

#endif
