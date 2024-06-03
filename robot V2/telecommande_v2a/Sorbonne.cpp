#include "Sorbonne.h"

#include <Arduino.h>

const int Sorbonne::_pinGripp[NBPINGRIPP] = {39, 37};
const int Sorbonne::_pinAux[NBPINAUX] = {33, 31, 29, 27, 25, 23};

Sorbonne::Sorbonne()
{
  
}

void Sorbonne::init()
{
  for (int i=0; i<NBPINGRIPP; i++)
  {
    pinMode(_pinGripp[i], INPUT_PULLUP);
    _gripp[i] = false;
    _gripp_p[i] = false;
  }
  for (int i=0; i<NBPINAUX; i++)
  {
    pinMode(_pinAux[i], INPUT_PULLUP);
    _aux[i] = false;
    _aux_p[i] = false;
  }
  _tp = millis();
}

void Sorbonne::run()
{
  unsigned long t = millis();
  if ((t-_tp)>50)
  {
    for (int i=0; i<NBPINGRIPP; i++)
    {
      _gripp_p[i] = _gripp[i];
      _gripp[i] = !digitalRead(_pinGripp[i]);
    }
    for (int i=0; i<NBPINAUX; i++)
    {
      _aux_p[i] = _aux[i];
      _aux[i] = !digitalRead(_pinAux[i]);
    }
    _tp=t;
  }
}

bool Sorbonne::getGripp(BoutonGripp b, Front f)
{
  switch (f)
  {
  case MONTANT : 
    return (!_gripp_p[b] && _gripp[b]);
  case DESCENDANT : 
    return (_gripp_p[b] && !_gripp[b]);
  case LESDEUX : 
    return (_gripp_p[b] != _gripp[b]);
  default:
  case ETAT:
    return _gripp[b];
  }
}

bool Sorbonne::getAux(BoutonAux b, Front f)
{
  switch (f)
  {
  case MONTANT : 
    return (!_aux_p[b] && _aux[b]);
  case DESCENDANT : 
    return (_aux_p[b] && !_aux[b]);
  case LESDEUX : 
    return (_aux_p[b] != _aux[b]);
  default:
  case ETAT:
    return _aux_p[b];
  }
}
