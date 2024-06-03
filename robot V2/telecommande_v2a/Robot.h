#ifndef ROBOT_H
#define ROBOT_H

class Robot
{
public:
  Robot();

  void init();
  void run();

  void deplacer(float cx, float cy, float cz);

private:
  void envoyerDeplacement(int d1, int d2, int d3);
  const int _pin_latch_vitesse = 45;
  
  float _VMAX, _RMAX;
  bool _precis;
};

#endif // JANOTUS_H
