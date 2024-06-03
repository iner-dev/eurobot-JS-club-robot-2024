#include "Janotus.h"
#include "Sorbonne.h"
#include "Robot.h"
#include "partie.h"
#include "bras.h"
#include "Points.h"
#include "animation.h"

//#define DEBUG_Janotus
//#define DEBUG_SORBONNE
//const int _pin_latch_equipe = 45;

Codeur C1(2, 32, false,false);
Led_Matrix M1(53,51,49);
Led_Matrix M2(52,50,48);
Janotus janotus(A9, A8, A10, 65);
Sorbonne sorbonne;
Robot robot;
Bras bras;
Partie partie;

animation anim(M1,M2);
Points points(C1, M1, M2);

float x, y, z;

void setup() {
  Serial.begin(115200);

  robot.init();
  partie.init();
  sorbonne.init();
  anim.start();
  janotus.init();
}

void loop() {
  partie.run();
  janotus.run();
  sorbonne.run();

  x=janotus.getX();
  y=janotus.getY();
  z=janotus.getZ();
  
  #ifdef DEBUG_Janotus
  Serial.print("Janotus : (");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.print(z);
  Serial.println(")");
  #endif
/*
  #ifdef DEBUG_SORBONNE
  Serial.print("Gripp_Orange : ");
  Serial.println(sorbonne.getGripp(Sorbonne::GRIPP_ORANGE));
  Serial.print("Gripp_Jaune : ");
  Serial.println(sorbonne.getGripp(Sorbonne::GRIPP_JAUNE));
  Serial.print("Aux_Orange : ");
  Serial.println(sorbonne.getAux(Sorbonne::AUX_ORANGE));
  Serial.print("Aux_Noir : ");
  Serial.println(sorbonne.getAux(Sorbonne::AUX_NOIR));
  Serial.print("Aux_Jaune : ");
  Serial.println(sorbonne.getAux(Sorbonne::AUX_JAUNE));
  Serial.print("Aux_Bleu : ");
  Serial.println(sorbonne.getAux(Sorbonne::AUX_BLEU));
  Serial.print("Aux_Blanc : ");
  Serial.println(sorbonne.getAux(Sorbonne::AUX_BLANC));
  Serial.print("Aux_Rouge : ");
  Serial.println(sorbonne.getAux(Sorbonne::AUX_ROUGE));

  #endif
*/
  robot.deplacer(x, y, z);

  if(anim.get_state()){
    anim.run();
  }else if(anim.ended()){
    points.init();
  }else{
    points.run();
  }

  if (!partie.armed && sorbonne.getAux(Sorbonne::AUX_ROUGE)){
    janotus.soft_calibrage();
  }

  if (sorbonne.getGripp(Sorbonne::GRIPP_ORANGE, Sorbonne::MONTANT))
    bras.actionnerPince(Bras::OUVERTE);
  else if (sorbonne.getGripp(Sorbonne::GRIPP_JAUNE, Sorbonne::MONTANT))
    bras.actionnerPince(Bras::FERMEE);
    
  if (sorbonne.getAux(Sorbonne::AUX_ORANGE))
    bras.deplacer(Bras::BAS); 
  else if (sorbonne.getAux(Sorbonne::AUX_NOIR))
    bras.deplacer(Bras::MILIEU);   
  else if (sorbonne.getAux(Sorbonne::AUX_JAUNE))
    bras.deplacer(Bras::HAUT);
  else if (sorbonne.getAux(Sorbonne::AUX_BLEU, Sorbonne::MONTANT))
    Serial1.println("R");

//  delay(10);
}
