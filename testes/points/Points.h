#ifndef Points_h
#define Points_h
#include <Arduino.h>
#include "Codeur.h"
#include "Led_Matrix.h"

class Points{
public:
  Points(Codeur &c,Led_Matrix &m1, Led_Matrix &m2);
  void init();
  void run();
  void update();
private:
  void run_get_input();
  Codeur &_codeur;
  Led_Matrix &_M1, &_M2;
  const int _pin_bouton_plus = 11;
  const int _pin_bouton_moin = 12;
  const int _pin_bouton_X10 = 12;
  bool _state_bouton_moin = false, _state_bouton_plus = false;
  int _points = 0;
  int _last_points = 0;
  void write_val_on_matrix (int val);
};

#endif
