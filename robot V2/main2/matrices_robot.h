#ifndef matrices_robot
#define matrices_robot

#include <LedControl.h>
#include "askii.h"



class Matrices_robot{
public:
  Matrices_robot();
  void init();
  void run();
  void update_serial_input(char index, int val);
  void write_txt(char txt[]);
  void write_num(int points);
private:
  LedControl matriceLed = LedControl(53/*DIN_pin*/, 49/*CLK_pin*/, 51/*CS_pin*/, 3);
  void init_matt(int index);
  void afficher_sur_mattrice(byte indexMatrice, byte pointeurVersChiffreOuSymbole[]);
  int char_To_Askii_Art(char x);
  unsigned long _animation_T0;
  unsigned int _points = 0;
  const int _animation_fps = 5;
  const int _animation_frames = 16;
  unsigned long _frame;
  int _last_frame = -1;
  int _last_points = -1;
};

#endif
