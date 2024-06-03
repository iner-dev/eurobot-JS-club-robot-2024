#include "matrices_robot.h"

Matrices_robot::Matrices_robot(){
  //
}

void Matrices_robot::update_serial_input(char index, int val){
  if(index =='p'){ 
    _points = val;
  }else if(index =='A'){
    _animation_T0 = millis();
  }
}

void Matrices_robot::init(){
  init_matt(0);
  init_matt(1);
  init_matt(2);
  _animation_T0 = millis();
}

void Matrices_robot::run(){
  _frame = _animation_fps*(millis() - _animation_T0)/1000;
  if(_frame < _animation_frames){
    if(_last_frame != _frame){
      _last_frame = _frame;
      afficher_sur_mattrice(0,animation_demarage_part1[_frame]);
      afficher_sur_mattrice(1,animation_demarage_part2[_frame]);
      afficher_sur_mattrice(2,animation_demarage_part3[_frame]);
    }
  }else{
    if(_last_points != _points){
      _last_points = _points;
      write_num(_points);
    }
  }
}

void Matrices_robot::init_matt(int index) {
  matriceLed.shutdown(index, false);       // shutdown     : "true" active le mode "sommeil", tandis que "false" active le mode "normal"
  matriceLed.setIntensity(index, 5);       // setIntensity : valeur pouvant aller de 0 à 15 (pour 16 niveaux de luminosité, donc)
  matriceLed.clearDisplay(index);          // clearDisplay : éteint toutes les LEDs de la matrice
}

void Matrices_robot::afficher_sur_mattrice(byte indexMatrice, byte pointeurVersChiffreOuSymbole[]) {
  // Parcours des 8 lignes
  for (int ligne = 0; ligne < 8; ligne++){
    // Et affichage des 8 colonnes pour chacune de ces lignes, avec fonction :
    // setRow(numero_de_matrice, numero_de_ligne, valeur_pour_cette_ligne)
    matriceLed.setRow(indexMatrice, ligne, pgm_read_byte_near(pointeurVersChiffreOuSymbole + ligne));
  }
}

void Matrices_robot::write_txt(char txt[]){
  for(int i = 0;i < 3;i++){
    afficher_sur_mattrice(i,char_To_Askii_Art(txt[i]));
  }
}

void Matrices_robot::write_num(int points){
  afficher_sur_mattrice(2,lettres[points%10 + 26]);
  if(int(points/100)%10 == 0){
    afficher_sur_mattrice(0,nothing);
    if(int(points/10)%10 == 0){
      afficher_sur_mattrice(1,nothing);
    }else{
      afficher_sur_mattrice(1,lettres[(points/10)%10 + 26]);
    }
  }else{
    afficher_sur_mattrice(0,lettres[(points/100)%10 + 26]);
    afficher_sur_mattrice(1,lettres[(points/10)%10 + 26]);
  }
}

int Matrices_robot::char_To_Askii_Art(char x){
  for(int i = 0; i < sizeof(inclued_char)/sizeof(inclued_char[0]); i++){
    if(inclued_char[i] == x){
      return lettres[i];
    }
  }
}
