#include <Servo.h>

Servo m_vis;
Servo m_grip;
Servo m_bras;

#define pin_fourche 3
#define pin_vis 9
#define pin_grip 5
#define pin_bras 6
#define pin_button 12
#define hauteur_max 15.75
#define vitesse_vis 45
#define stop_speed 91.5

float hauteur = 0;
float wanted_hauteur = 15; // 4,5:grab plante 5,5:grab pot 4,5:pot_bas 10:pos_haut 14:plante_haut
float pos_bras = 45;
float wanted_pos_bras = 45; // 4,5:grab plante 5,5:grab pot 4,5:pot_bas 10:pos_haut 14:plante_haut, 
int v_vis = 90;
char input;
int actual_prog = 0;// -1 : manuel, 0 : inactif, 1 : prog recup pot, 2 : prog recup plante, 3 : prog deposer pot, 4 : prog deposer pot jardiniere
int phase = 0;
int delay_bras_speed = 1;
int step_bras_speed = 1;
unsigned long debut;
unsigned long last_check;

void instant_pos_bras(int pos){
  m_bras.write(pos);
  wanted_pos_bras = pos;
  pos_bras = pos;
}


void go_home(){
  v_vis = 180;
  m_vis.write(180);
  while (digitalRead(pin_button)==false) {
    delay(1);
  }
  m_vis.write(stop_speed);
  hauteur = hauteur_max;
  wanted_hauteur = 15;
  v_vis = stop_speed;
}

void fourche_change(){
  if(v_vis != stop_speed){
    if(v_vis > stop_speed){
      hauteur = hauteur + 0.125;
    }else{
      hauteur = hauteur - 0.125;
    }
  }
  if(hauteur == wanted_hauteur){
    delay(10);
    v_vis = stop_speed;
    m_vis.write(v_vis);
  }
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pin_fourche), fourche_change, CHANGE);
  last_check = millis();
  m_vis.attach(pin_vis);
  m_grip.attach(pin_grip);
  m_bras.attach(pin_bras);

  pinMode(pin_fourche, INPUT);
  pinMode(pin_button, INPUT_PULLUP);
  m_grip.write(45);
  m_bras.write(45);
  
  go_home();
}

void loop() {
  if(Serial.available()){ // check les messages recus
    input = Serial.read();
    if(input == 'v'){
      wanted_hauteur = Serial.parseFloat();
      actual_prog = -1;
      delay_bras_speed = 1;
    }
    if(input == 'h'){
      hauteur = -15;
    }
    if(input == 'G'){
      m_grip.write(Serial.parseInt());
      actual_prog = -1;
    }
    if(input == 'b'){
      instant_pos_bras(Serial.parseInt());
      actual_prog = -1;
    }
    if(input =='P' and actual_prog == 0){
      actual_prog = Serial.parseInt();
      phase = 0;
    }
    Serial.println(input);
  }
  
  if(actual_prog == 1){
    if(phase == 0){ // initiation de la 1ere mouvement 
      phase = 1;
      debut = millis();
      wanted_hauteur = 2;
      m_grip.write(80);
      instant_pos_bras(55);
    }else if(phase == 1 and (millis()-debut)>12000){// check si la 1ere mouvement fini
      phase = 3;
      debut = millis();
      m_grip.write(20);
      wanted_hauteur = 15;
    }else if(phase == 3 and (millis()-debut)>5000){ // initiation du 3eme mouvement 
      phase = 4;
      debut = millis();
      wanted_pos_bras = (115);
      wanted_hauteur = 5;
      delay_bras_speed = 40;
    }else if(phase == 4 and (millis()-debut)>3000){ // initiation de la 4eme mouvement 
      phase = 5;
      debut = millis();
      wanted_pos_bras = (105);
      m_grip.write(55);
    }else if(phase == 5 and (millis()-debut)>10){ // initiation de la 5eme mouvement 
      phase = 6;
      debut = millis();
      wanted_hauteur = 15;
    }else if(phase == 6 and (millis()-debut)>3000){// check fin de programe
      phase = 0;
      actual_prog = 0;
      wanted_pos_bras = (45);
      wanted_hauteur = 15.5;
      delay_bras_speed = 1;
    }
  }
  
  if(actual_prog == 2){
    if(phase == 0){ // initiation de la 1ere mouvement 
      phase = 1;
      debut = millis();
      wanted_hauteur = 0.5;
      m_grip.write(80);
      instant_pos_bras(55);
    }else if(phase == 1 and (millis()-debut)>9000){// check si la 1ere mouvement fini
      phase = 2;
      debut = millis();
      m_grip.write(22);
    }else if(phase == 2 and (millis()-debut)>1000){ // initiation de la 2nd mouvement 
      phase = 3;
      debut = millis();
      wanted_hauteur = 15;
    }else if(phase == 3 and (millis()-debut)>9000){ // initiation du 3eme mouvement 
      phase = 4;
      debut = millis();
      wanted_pos_bras = (115);
      delay_bras_speed = 40;
    }else if(phase == 4 and (millis()-debut)>3000){ // initiation de la 4eme mouvement 
      phase = 5;
      debut = millis();
      wanted_pos_bras = (105);
      m_grip.write(55);
    }else if(phase == 5 and (millis()-debut)>10){ // initiation de la 5eme mouvement 
      phase = 6;
      debut = millis();
      wanted_hauteur = 15;
    }else if(phase == 6 and (millis()-debut)>3000){// check fin de programe
      phase = 0;
      actual_prog = 0;
      instant_pos_bras(45);
      wanted_hauteur = 15.5;
    }
  }

  if(actual_prog == 3){
    if(phase == 0){ // initiation de la 1ere mouvement 
      phase = 1;
      debut = millis();
      wanted_hauteur = 6;
      instant_pos_bras(80);
      m_grip.write(60);
    }else if(phase == 1 and (millis()-debut)>7000){// check si la 1ere mouvement fini
      phase = 2;
      debut = millis();
      wanted_pos_bras = (115);
      delay_bras_speed = 40;
    }else if(phase == 2 and (millis()-debut)>2000){ // initiation de la 2nd mouvement 
      phase = 3;
      debut = millis();
      m_grip.write(35);
    }else if(phase == 3 and (millis()-debut)>500){ // initiation du 3eme mouvement 
      phase = 4;
      debut = millis();
      wanted_hauteur = 6;
    }else if(phase == 4 and (millis()-debut)>2000){ // initiation de la 4eme mouvement 
      phase = 6;
      debut = millis();
      wanted_pos_bras = (45);
      delay_bras_speed = 10;
      wanted_hauteur = 1.5;
    }else if(phase == 6 and (millis()-debut)>5000){// check fin de programe
      phase = 0;
      actual_prog = 0;
      m_grip.write(55);
      wanted_hauteur = 15.5;
      instant_pos_bras(45);
    }
  }
  
  if(actual_prog == 4){
    if(phase == 0){ // initiation de la 1ere mouvement 
      phase = 1;
      debut = millis();
      wanted_hauteur = 6.5;
      instant_pos_bras(80);
      m_grip.write(60);
    }else if(phase == 1 and (millis()-debut)>7000){// check si la 1ere mouvement fini
      phase = 2;
      debut = millis();
      wanted_pos_bras = (115);
      delay_bras_speed = 40;
    }else if(phase == 2 and (millis()-debut)>2000){ // initiation de la 2nd mouvement 
      phase = 3;
      debut = millis();
      m_grip.write(35);
    }else if(phase == 3 and (millis()-debut)>500){ // initiation du 3eme mouvement 
      phase = 4;
      debut = millis();
      wanted_hauteur = 12;
    }else if(phase == 4 and (millis()-debut)>4500){ // initiation de la 4eme mouvement 
      phase = 5;
      debut = millis();
      wanted_pos_bras = (45);
      delay_bras_speed = 40;
    }else if(phase == 5 and (millis()-debut)>5000){ // initiation de la 5eme mouvement 
      phase = 6;
      debut = millis();
      wanted_hauteur = 2;
    }else if(phase == 6 and (millis()-debut)>5000){// check fin de programe
      phase = 0;
      actual_prog = 0;
      m_grip.write(55);
      wanted_hauteur = 15.5;
      instant_pos_bras(45);
    }
  }
  
  
  if(hauteur != wanted_hauteur){ //check de la hauteur de la pince

    if(hauteur > wanted_hauteur){
      v_vis = stop_speed - vitesse_vis;
    }else{
      v_vis = stop_speed + vitesse_vis;
    }
    m_vis.write(v_vis);
  }
  if(digitalRead(pin_button)){
    hauteur = hauteur_max;
  }
  if(pos_bras != wanted_pos_bras and (millis() - last_check) > 40){
    last_check = millis();
    if(pos_bras > wanted_pos_bras){
      pos_bras -= step_bras_speed;
    }else{
      pos_bras += step_bras_speed;
    }
    m_bras.write(pos_bras);
  }
}
