#ifndef anim_h
#define anim_h
#include "Arduino.h" 
#include "Led_Matrix.h"

class animation{
public:
  animation(Led_Matrix &M1, Led_Matrix &M2);
  void run();
  void start();
  bool state = false;
private:
  Led_Matrix &_M1, &_M2;
  unsigned long _T0;
  int _frame;
  int _last_frame;
  int get_frame();
  const int _FPS = 6;
  const int _frames_number = 18;
};

#endif
