#ifndef SORBONNE_H
#define SORBONNE_H

#define NBPINGRIPP 2
#define NBPINAUX 6

class Sorbonne
{
public:
  enum BoutonGripp
  {
    GRIPP_ORANGE = 0,
    GRIPP_JAUNE = 1
  };
  enum BoutonAux
  {
    AUX_ORANGE = 0,
    AUX_NOIR = 1,
    AUX_JAUNE = 2,
    AUX_BLEU = 3,
    AUX_BLANC = 4,
    AUX_ROUGE = 5
  };
  enum Front 
  {
    ETAT = 0,
    MONTANT,
    DESCENDANT,
    LESDEUX
  };
  Sorbonne();

  void init();
  void run();

  bool getGripp(BoutonGripp b, Front f=ETAT);
  bool getAux(BoutonAux b, Front f=ETAT);
private:
  static const int Sorbonne::_pinGripp[NBPINGRIPP];
  static const int Sorbonne::_pinAux[NBPINAUX];
  int _gripp[NBPINGRIPP];
  int _aux[NBPINAUX];
  int _gripp_p[NBPINGRIPP];
  int _aux_p[NBPINAUX];
  unsigned long _tp;
};

#endif // SORBONNE_H
