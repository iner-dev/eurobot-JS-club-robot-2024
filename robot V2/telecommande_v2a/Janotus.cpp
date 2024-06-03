#include "Janotus.h"

#include <Arduino.h>

Janotus::Janotus(int pinX, int pinY, int pinZ, int Z_less) : _pinX(pinX), _pinY(pinY), _pinZ(pinZ), _Z_less(Z_less)
{
  _x = 0;
  _y = 0;
  _z = 0;
 /* Janotus v1
  _xMin = -0.76;
  _xMax = -0.12;
  _yMin = -0.45;
  _yMax = 0.75;
  _zMin = 0.22;
  _zMax = 0.96;
  */
 // Janotus v
  _xMin = -0.20;
  _xMax = 0.50;
  _yMin = -0.98;
  _yMax = 0.18;
  _zMin = -0.25;
  _zMax = 0.48;
  
  _Px = 0.10;
  _Py = 0.10;
  _Pz = 0.35;
  _Cx = 0;
  _Cy = 0;
  _Cz = 0;
}
/*
void Janotus::soft_calibrage(){
  _Cx = (2.*(((float)analogRead(_pinX))/512.-1.)-_xMax-_xMin)/(_xMax-_xMin);
  _Cy = (2.*(((float)analogRead(_pinY))/512.-1.)-_yMax-_yMin)/(_yMax-_yMin);
  _Cz = (2.*(((float)analogRead(_pinZ))/512.-1.)-_zMax-_zMin)/(_zMax-_zMin);
}

void Janotus::run()
{
  float x=((float)analogRead(_pinX))/512.-1.;
  if ((x>=_xMin) && (x<=_xMax))
  {
    _x=-((2.*x-_xMax-_xMin)/(_xMax-_xMin) - _Cx);
    if (_x > _Px)
      _x = (_x - _Px)/(1 - _Px);
    else if (_x < -_Px)
      _x = (_x + _Px)/(1 - _Px);
    else
      _x = 0; 
 //   _x=_x*_x*_x;
  } 

  float y=((float)analogRead(_pinY))/512.-1.;
  if ((y>=_yMin) && (y<=_yMax))
  {
    _y=(2.*y-_yMax-_yMin)/(_yMax-_yMin) - _Cy;
    if (_y > _Py)
      _y = (_y - _Py)/(1 - _Py);
    else if (_y < -_Py)
      _y = (_y + _Py)/(1 - _Py);
    else
      _y = 0; 
 //   _y=_y*_y*_y;
  }
  

  float z=((float)analogRead(_pinZ))/512.-1.;
  if ((z>=_zMin) && (z<=_zMax))
  {
    _z=(2.*z-_zMax-_zMin)/(_zMax-_zMin) - _Cz;
    if (_z > _Pz)
      _z = (_z - _Pz)/(1 - _Pz);
    else if (_z < -_Pz)
      _z = (_z + _Pz)/(1 - _Pz);
    else
      _z = 0; 
 //   _z=_z*_z*_z;
  }
  #ifdef calibrage
  Serial.print("x = ");
  Serial.print(x);
  Serial.print("| y = ");
  Serial.print(y);
  Serial.print("| z = ");
  Serial.println(z);
  #endif
}
*/

void Janotus::init(){
  pinMode(_Z_less,INPUT_PULLUP);
}

void Janotus::soft_calibrage(){
  _Cx = -(2.*(((float)analogRead(_pinX))/512.-1.)-_xMax-_xMin)/(_xMax-_xMin);
  _Cy = (2.*(((float)analogRead(_pinY))/512.-1.)-_yMax-_yMin)/(_yMax-_yMin);
  _Cz = (2.*(((float)analogRead(_pinZ))/512.-1.)-_zMax-_zMin)/(_zMax-_zMin);
}

void Janotus::run()
{
  bool boost = false;
  
  float x=((float)analogRead(_pinX))/512.-1.;
  #ifdef calibrage
  Serial.print("x = ");
  Serial.print(x);
  #endif
  if (x<_xMin)
    _x=1.;
  else if (x>_xMax)
    _x=-1.;
  else
  {
    _x=-(2.*x-_xMax-_xMin)/(_xMax-_xMin);
    if (_x > (_Cx + _Px))
      _x = (_x - (_Cx + _Px))/(1 - (_Cx + _Px));
    else if (_x < (_Cx -_Px))
      _x = (_x - (_Cx -_Px))/((_Cx -_Px) + 1);
    else
      _x = 0; 
  } 

  float y=((float)analogRead(_pinY))/512.-1.;
  #ifdef calibrage
  Serial.print("| y = ");
  Serial.print(y);
  #endif
  if (y<_yMin)
  {
    _y=-1.;
    boost = false;
  }
  else if (y>_yMax) 
  {
    boost = true;
    _y=1.;
  }
  else
  {
    _y=(2.*y-_yMax-_yMin)/(_yMax-_yMin);
    if (_y > (_Cy + _Py))
      _y = (_y - (_Cy + _Py))/(1 - (_Cy + _Py));
    else if (_y < (_Cy -_Py))
      _y = (_y - (_Cy -_Py))/((_Cy -_Py) + 1);
    else
      _y = 0; 
    boost = false;
  }

  float z=((float)analogRead(_pinZ))/512.-1.;
  #ifdef calibrage
  Serial.print("| z = ");
  Serial.println(z);
  #endif
  if (!digitalRead(_Z_less)){
    _z=0;
  }else if (z<_zMin){
    _z=-1.;
  } else if (z>_zMax) {
    _z=1.;
  } else
  {
    _z=(2.*z-_zMax-_zMin)/(_zMax-_zMin);
    if (_z > (_Cz + _Pz))
      _z = (_z - (_Cz + _Pz))/(1 - (_Cz + _Pz));
    else if (_z < (_Cz -_Pz))
      _z = (_z - (_Cz -_Pz))/((_Cz -_Pz) + 1);
    else
      _z = 0; 
    _z=_z*_z*_z;
  }

  if (boost)
  {
    _x=0.;
    _y=1.5;
    _z=0.;
  }
}

float Janotus::getX()
{
  return _x;
}

float Janotus::getY()
{
  return _y;
}

float Janotus::getZ()
{
  return _z;
}
