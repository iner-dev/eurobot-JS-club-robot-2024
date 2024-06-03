#ifndef Bras_h
#define Bras_h
#include <Arduino.h>

class Bras{
public:

  enum PositionBras {
    BAS = 0,
    MILIEU = 1,
    HAUT = 2
  };
  
  enum PositionPince {
    OUVERTE = 0,
    INTERMEDIAIRE = 1,
    FERMEE = 2
  };
  
  Bras();
  void init();
  void run();

  void actionnerPince(PositionPince p);

  void deplacer(PositionBras p);
private:
};

#endif
