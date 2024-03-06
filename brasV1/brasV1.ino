#include <Servo.h>

Servo m_vis;
Servo m_grip;
Servo m_bras;

#define pin_fourche 2
#define pin_vis 3
#define pin_grip 4
#define pin_bras 5
#define pin_button 6

float hauteur = 70;
float wanted_hauteur = 70;
int v_vis = 90;
char input;
int actual_prog = 0;// -1 : manuel, 0 : inactif, 1 : prog recup pot, 2 : prog recup plante, 3 : prog deposer pot
int phase = 0;
unsigned long debut;


void fourche_change(){
  if(v_vis > 90){
    hauteur += 0.125;
  }else{
    hauteur -= 0.125;
  }
  if(hauteur == wanted_hauteur){
    v_vis = 90;
    m_vis.write(v_vis);
  }
}

void setup() {
  Serial.begin(9600);
  m_vis.attach(pin_vis);
  m_grip.attach(pin_grip);
  m_bras.attach(pin_bras);

  pinMode(pin_fourche, INPUT);
  pinMode(pin_button, INPUT);
  
  m_vis.write(180);
  while (digitalRead(pin_button)) {}
  m_vis.write(90);
  
  attachInterrupt(digitalPinToInterrupt(pin_fourche), fourche_change, CHANGE);
}

void loop() {
  if(Serial.available()){ // check les messages recus
    input = Serial.read();
    if(input == 'v'){
      wanted_hauteur = Serial.parseInt();
      actual_prog = -1;
    }
    if(input == 'G'){
      m_grip.write(Serial.parseInt());
      actual_prog = -1;
    }
    if(input == 'b'){
      m_bras.write(Serial.parseInt());
      actual_prog = -1;
    }
    if(input =='P' and actual_prog == 0){
      actual_prog = Serial.parseInt();
      phase = 0;
    }
  }
  
  if(actual_prog == 1){
    if(phase == 0){ // initiation de la 1ere mouvement 
      phase = 1;
      debut = millis();
      wanted_hauteur = 10;
      m_grip.write(20);
      m_bras.write(90);
    }else if(phase == 1 and (millis(debut)-)>1000){// check si la 1ere mouvement fini
      phase = 2;
    }else if(phase == 2){ // initiation de la 2nd mouvement 
      phase = 3;
      debut = millis();
      wanted_hauteur = 10;
      m_grip.write(20);
      m_bras.write(90);
    }else if(phase == 3 and (millis(debut)-)>1000){// check fin de programe
      phase = 0;
      actual_prog == 0
    }
  }
  
  if(hauteur != wanted_hauteur){ //check de la hauteur de la pince
    if(hauteur > wanted_hauteur){
      v_vis = 180;
      m_vis.write(v_vis);
    }else{
      v_vis = 0;
      m_vis.write(v_vis);
    }
  }
}
