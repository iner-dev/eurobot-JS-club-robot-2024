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
  if(Serial.available()){
    input = Serial.read();
    if(input == 'v'){
      wanted_hauteur = Serial.parseInt();
    }
    if(input == 'G'){
      m_grip.write(Serial.parseInt());
    }
    if(input == 'b'){
      m_bras.write(Serial.parseInt());
    }
  }
  if(hauteur != wanted_hauteur){
    if(hauteur > wanted_hauteur){
      v_vis = 180;
      m_vis.write(v_vis);
    }else{
      v_vis = 0;
      m_vis.write(v_vis);
    }
  }
}
