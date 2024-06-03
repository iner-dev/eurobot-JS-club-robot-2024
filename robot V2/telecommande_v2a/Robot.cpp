#include "Robot.h"

#include <Arduino.h>

Robot::Robot()
{
  _precis=false;
  _VMAX = 9000;
  _RMAX = 3000;
}

void Robot::init()
{
  Serial1.begin(38400);
  pinMode(_pin_latch_vitesse,INPUT_PULLUP);
}


void Robot::run()
{
  
}

void Robot::deplacer(float cx, float cy, float cz)
{
  float theta=atan2(cx, cy);
  float module = sqrt(cx*cx+cy*cy);
/*  
  Serial.print("Robot : (");
  Serial.print(theta);
  Serial.print(", ");
  Serial.print(module);
  Serial.println(")");
 */

  float v1 = -module*sin(theta+PI/3.);
  float v2 = module*sin(theta-PI/3.);
  float v3 = module*sin(theta);
 /* 
  Serial.print("Robot : (");
  Serial.print(v1);
  Serial.print(", ");
  Serial.print(v2);
  Serial.print(", ");
  Serial.print(v3);
  Serial.println(")");
*/
  if (digitalRead(_pin_latch_vitesse))
    envoyerDeplacement((v1*_VMAX+cz*_RMAX)/4, (v2*_VMAX-cz*_RMAX)/4, (v3*_VMAX+cz*_RMAX)/4);
  else
    envoyerDeplacement(v1*_VMAX+cz*_RMAX, v2*_VMAX-cz*_RMAX, v3*_VMAX+cz*_RMAX);
}

void Robot::envoyerDeplacement(int d1, int d2, int d3)
{
  Serial1.println("1"+String(d1));
  Serial1.println("2"+String(d2));
  Serial1.println("3"+String(d3));
  Serial1.println("u");
/* Serial1.println("11000");
  Serial1.println("21000");
  Serial1.println("31000");
  Serial1.println("u");*/
/*  Serial.print("1 "+String(d1));
  Serial.print(", 2 "+String(d2));
  Serial.println(", 3 "+String(d3));//*/
}
