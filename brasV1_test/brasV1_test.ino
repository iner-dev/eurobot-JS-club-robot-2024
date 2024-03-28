#include <Servo.h>

Servo m_vis;

#define pin_fourche 3
#define pin_vis 9
#define pin_button 12

float hauteur = 70;
float wanted_hauteur = 70;
int v_vis = 90;



void setup() {
  Serial.begin(9600);
  m_vis.attach(pin_vis);

  pinMode(pin_fourche, INPUT);
  pinMode(pin_button, INPUT_PULLUP);
  
  //m_vis.write(0);
  //while (!digitalRead(pin_button)) {}
  m_vis.write(90);
}

void loop() {
}
