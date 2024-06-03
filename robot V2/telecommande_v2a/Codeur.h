#ifndef CODEUR_H
#define CODEUR_H

#include <Arduino.h>

class Codeur
{
public: 
  Codeur(int aPin, int bPin, bool invert = false, bool mode_unsigned = false);

  void init();

  float getPosition();
  float getVitesse();
  void set_Position(int);
private:
  int _aPin;
  int _bPin;
  bool _invert;
  bool _mode_unsigned;
  volatile byte a, ap;
  volatile byte b;
  volatile long _pos;
  volatile long apos;
  volatile float _vit;
  static Codeur * _interruptInstance[2];
  volatile unsigned long t;
  volatile unsigned long tp;

  void interrupt();

  friend void interruptionCodeur0();
  friend void interruptionCodeur1();
};

#endif
