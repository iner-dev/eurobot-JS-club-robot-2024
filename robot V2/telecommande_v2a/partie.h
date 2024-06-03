#ifndef partie_h
#define partie_h
#include <Arduino.h>

class Partie{
public:
  Partie();
  void init();
  void run();
  void start_pami();
  bool armed = false;
private:
  void start();
  void time_check();
  void button_check();
  unsigned long _t0;
  unsigned long _time;
  const long _demarage_PAMI = 90000;
  const long _fin = 99000;
  bool _started = false;
  bool _PAMI_start_sent = false;
  const int _pin_latch_desarmement = 5;
  const int _pin_bouton_start_pami = 47;
  const int _pin_bouton_start = 22;
};

#endif
