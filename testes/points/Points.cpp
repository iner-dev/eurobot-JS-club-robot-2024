#include "Points.h"

Points::Points(Codeur &c,Led_Matrix &m1,Led_Matrix &m2): _codeur(c),_M1(m1), _M2(m2){
  //
}

void Points::init(){
  _codeur.init();
  pinMode(_pin_bouton_plus,INPUT_PULLUP);
  pinMode(_pin_bouton_moin,INPUT_PULLUP);
  update();
}

void Points::run_get_input(){
  if(_codeur.getPosition() != 0){
    _points = _points + _codeur.getPosition();
    _codeur.set_Position(0);
  }
  if(!digitalRead(_pin_bouton_plus)){
    if(_state_bouton_plus){
      _points = _points + 1;
      _state_bouton_plus = false;
    }
  }else{
    _state_bouton_plus = true;
  }
  if(!digitalRead(_pin_bouton_moin)){
    if(_state_bouton_moin){
      _points = _points -1;
      _state_bouton_moin = false;
    }
  }else{
    _state_bouton_moin = true;
  }
  if(_points<0){
    _points=0;
  }
}

void Points::run(){
  run_get_input();
  if(_points != _last_points){
    _last_points = _points;
    update();
  }
}

void Points::update(){
  Serial1.println("p"+String(_points));
  write_val_on_matrix(_points);
}

void Points::write_val_on_matrix(int val){
   _M2.i_Write(_M2.chiffres[val%10]);
   if(int(val/10)%10!=0){
    _M1.i_Write(_M1.chiffres[int(val/10)%10]);
   }else{
    _M1.i_Write(_M1.nothing);
   }
}
