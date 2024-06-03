#include "Codeur.h"

Codeur * Codeur::_interruptInstance[2] = { NULL, NULL};

void interruptionCodeur0()
{
  if (Codeur::_interruptInstance[0])
    Codeur::_interruptInstance[0]->interrupt();
}

void interruptionCodeur1()
{
  if (Codeur::_interruptInstance[1])
    Codeur::_interruptInstance[1]->interrupt();
}

Codeur::Codeur(int aPin, int bPin, bool invert, bool mode_unsigned) : _aPin(aPin), _bPin(bPin), _invert(invert), _mode_unsigned(mode_unsigned), _pos(0L)
{
  if (aPin == 2)
    _interruptInstance[0] = this;
  else if (aPin == 3)
    _interruptInstance[1] = this;
}

void Codeur::init()
{
  pinMode(_aPin, INPUT);
  pinMode(_bPin, INPUT);
  ap = digitalRead(_aPin);
  _pos = 0L;
  apos = 0L;
  tp = 0L;
  _vit = 0.;
  if (_aPin == 2)
    attachInterrupt(0, interruptionCodeur0, CHANGE);
  else if (_aPin == 3)
    attachInterrupt(1, interruptionCodeur1, CHANGE);
}

float Codeur::getPosition()
{
  return _pos;
}

float Codeur::getVitesse()
{
  t = micros();
  if ((t-tp) > 50000)
    return 0;
  float v;
  cli();
  v=_vit;
  sei();
  return _vit;
}

void Codeur::interrupt()
{
  a = digitalRead(_aPin);
  if (a!=ap)
  {
    ap=a;
    if(a==true)
    {
      t = micros();
      b= digitalRead(_bPin);
      if (a==b)
        _pos += (_invert) ? 1 : -1;
      else
        _pos -= (_invert) ? 1 : -1; 
      if (_mode_unsigned and _pos < 0 ){
        _pos = 0;
      }
      _vit= (float)(_pos-apos)/(float)(t-tp)*14960.;
      apos=_pos;
      tp=t;
    }
  }
}
void Codeur::set_Position(int value)
{
  _pos = value;
}
