#ifndef JANOTUS_H
#define JANOTUS_H

//#define calibrage

class Janotus
{
public:
  Janotus(int pinX, int pinY, int pinZ, int Z_less);

  void run();
  void soft_calibrage();
  float getX();
  float getY();
  float getZ();
  void init();

private:
  int _pinX, _pinY, _pinZ, _Z_less;
  float _x, _y, _z;
  float _xMin, _xMax, _yMin, _yMax, _zMin, _zMax, _Px, _Py, _Pz, _Cx, _Cy, _Cz;
};

#endif // JANOTUS_H
