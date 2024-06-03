#include "bras.h"

Bras::Bras()
{
  
}

void Bras::init()
{
  
}

void Bras::run()
{
  
}

void Bras::actionnerPince(Bras::PositionPince p)
{
  switch(p)
  {
  case OUVERTE:
    Serial1.println("G0");
    break;
  case INTERMEDIAIRE:
    Serial1.println("G220");
    break;
  case FERMEE:
    Serial1.println("G255");
    break;
  }
  
}

void Bras::deplacer(Bras::PositionBras p)
{
  switch (p)
  {
  case BAS:
    Serial1.print("b");
    Serial1.println(0);
    break;
  case MILIEU:
    Serial1.print("b");
    Serial1.println(50);
    break;
  default:
  case HAUT:
    Serial1.print("b");
    Serial1.println(255);
    break;
  }
  
}
