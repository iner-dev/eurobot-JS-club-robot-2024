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
#define vitesse_vis 80
#define stop_speed 92
#define pos_stop_vis 15.625
#define pos_stop_grip 55
#define pos_stop_bras 45


float hauteur = 0;
float wanted_hauteur = 15; // 4,5:grab plante 5,5:grab pot 4,5:pot_bas 10:pos_haut 14:plante_haut
float pos_bras = 110;
float wanted_pos_bras = 110; // 4,5:grab plante 5,5:grab pot 4,5:pot_bas 10:pos_haut 14:plante_haut,
int v_vis = 90;
char input;
int actual_prog = 0;// -1 : manuel, 0 : inactif, 1 : prog recup pot, 2 : prog recup plante, 3 : prog deposer pot, 4 : prog deposer pot jardiniere
int phase = 0;
int delay_bras_speed = 1;
int step_bras_speed = 1;
unsigned long debut;
unsigned long last_check;
bool armed = false;

void instant_pos_bras(int pos) {
  m_bras.write(pos);
  wanted_pos_bras = pos;
  pos_bras = pos;
}

void exec_prog() {
  if (actual_prog == 0) {
    phase = 0;
    wanted_pos_bras = pos_stop_bras;
    wanted_hauteur = pos_stop_vis;
    m_grip.write(pos_stop_grip);
    delay_bras_speed = 1;
  }
  if (actual_prog == 1) {
    if (phase == 0) { // initiation de la 1ere mouvement
      phase = 1;
      debut = millis();
      wanted_hauteur = 0.5;
      m_grip.write(78);
      instant_pos_bras(45);
    } else if (phase == 1 and (millis() - debut) > 4000) { // check si la 1ere mouvement fini
      phase = 3;
      debut = millis();
      wanted_pos_bras = (84);
      delay_bras_speed = 40;
    } else if (phase == 3 and (millis() - debut) > 3000) { // initiation du 3eme mouvement
      phase = 4;
      debut = millis();
      m_grip.write(27);
      wanted_hauteur = 8;
    } else if (phase == 4 and (millis() - debut) > 3000) { // initiation de la 4eme mouvement
      phase = 5;
      debut = millis();
      wanted_pos_bras = (117);
    } else if (phase == 5 and (millis() - debut) > 2000) { // initiation de la 5eme mouvement
      phase = 6;
      debut = millis();
      m_grip.write(60);
      wanted_pos_bras = (50);
    } else if (phase == 6 and (millis() - debut) > 2000) { // check fin de programe
      actual_prog = 0;
    }
  }

  if (actual_prog == 2) {
    if (phase == 0) { // initiation de la 1ere mouvement
      phase = 1;
      debut = millis();
      wanted_hauteur = 0.5;
      m_grip.write(78);
      instant_pos_bras(45);
    } else if (phase == 1 and (millis() - debut) > 5000) { // check si la 1ere mouvement fini
      phase = 3;
      debut = millis();
      wanted_pos_bras = (84);
      delay_bras_speed = 40;
    } else if (phase == 3 and (millis() - debut) > 3000) { // initiation du 3eme mouvement
      phase = 4;
      debut = millis();
      m_grip.write(22);
      wanted_hauteur = 15;
    } else if (phase == 4 and (millis() - debut) > 5000) { // initiation de la 4eme mouvement
      phase = 5;
      debut = millis();
      wanted_pos_bras = (125);
    } else if (phase == 5 and (millis() - debut) > 2000) { // initiation de la 5eme mouvement
      phase = 6;
      debut = millis();
      m_grip.write(60);
      wanted_pos_bras = (50);
    } else if (phase == 6 and (millis() - debut) > 2000) { // check fin de programe
      actual_prog = 0;
    }
  }

  if (actual_prog == 3) {
    if (phase == 0) { // initiation de la 1ere mouvement
      phase = 1;
      debut = millis();
      wanted_hauteur = 15.625;
      instant_pos_bras(116);
      m_grip.write(64);
      delay_bras_speed = 30;
    } else if (phase == 1 and (millis() - debut) > 100) { // check si la 1ere mouvement fini
      phase = 2;
      debut = millis();
      wanted_hauteur = 7.625;
    } else if (phase == 2 and (millis() - debut) > 3000) { // initiation de la 2nd mouvement
      phase = 3;
      debut = millis();
      m_grip.write(32);
      wanted_hauteur = 9;
    } else if (phase == 3 and (millis() - debut) > 2000) { // initiation du 3eme mouvement
      phase = 4;
      debut = millis();
      wanted_pos_bras = 72;
    } else if (phase == 4 and (millis() - debut) > 2000) { // initiation de la 4eme mouvement
      phase = 5;
      debut = millis();
      wanted_hauteur = 1.5;
    } else if (phase == 5 and (millis() - debut) > 3000) { // initiation de la 4eme mouvement
      phase = 6;
      debut = millis();
      m_grip.write(72);
      wanted_pos_bras = 44;
    } else if (phase == 6 and (millis() - debut) > 3000) { // check fin de programe
      actual_prog = 0;
    }
  }

  if (actual_prog == 4) {
    if (phase == 0) { // initiation de la 1ere mouvement
      phase = 1;
      debut = millis();
      wanted_hauteur = 15.625;
      instant_pos_bras(116);
      m_grip.write(64);
      delay_bras_speed = 30;
    } else if (phase == 1 and (millis() - debut) > 100) { // check si la 1ere mouvement fini
      phase = 2;
      debut = millis();
      wanted_hauteur = 7.625;
    } else if (phase == 2 and (millis() - debut) > 3000) { // initiation de la 2nd mouvement
      phase = 3;
      debut = millis();
      m_grip.write(32);
      wanted_hauteur = 15;
    } else if (phase == 3 and (millis() - debut) > 4000) { // initiation du 3eme mouvement
      phase = 4;
      debut = millis();
      wanted_pos_bras = 50;
    } else if (phase == 4 and (millis() - debut) > 2000) { // initiation de la 4eme mouvement
      phase = 5;
      debut = millis();
      wanted_hauteur = 1.5;
    } else if (phase == 5 and (millis() - debut) > 6000) { // initiation de la 4eme mouvement
      phase = 6;
      debut = millis();
      m_grip.write(72);
      wanted_pos_bras = 40;
    } else if (phase == 6 and (millis() - debut) > 3000) { // check fin de programe
      actual_prog = 0;
    }
  }
}

void exec_moteur() {
  if (hauteur != wanted_hauteur) { //check de la hauteur de la pince

    if (hauteur > wanted_hauteur) {
      v_vis = stop_speed - vitesse_vis;
    } else {
      v_vis = stop_speed + vitesse_vis;
    }
    m_vis.write(v_vis);
  }
  if (pos_bras != wanted_pos_bras and (millis() - last_check) > 40) {
    last_check = millis();
    if (pos_bras > wanted_pos_bras) {
      pos_bras -= step_bras_speed;
    } else {
      pos_bras += step_bras_speed;
    }
    m_bras.write(pos_bras);
  }
}

void go_home() {
  hauteur = -20;
  wanted_hauteur = pos_stop_vis;
}

void fourche_change() {
  if (v_vis != stop_speed) {
    if (v_vis > stop_speed) {
      hauteur = hauteur + 0.125;
    } else {
      hauteur = hauteur - 0.125;
    }
  }
  if (hauteur == wanted_hauteur) {
    delay(5);
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
  m_grip.write(pos_stop_grip);
  m_bras.write(pos_stop_bras);

  go_home();
}

void loop() {
  if (Serial.available()) { // check les messages recus
    input = Serial.read();
    if (armed) {
      if (input == 'v') {
        wanted_hauteur = Serial.parseFloat();
        actual_prog = -1;
        delay_bras_speed = 1;
      }
      if (input == 'h') {
        hauteur = -15;
      }
      if (input == 'G') {
        m_grip.write(Serial.parseInt());
        actual_prog = -1;
      }
      if (input == 'b') {
        instant_pos_bras(Serial.parseInt());
        actual_prog = -1;
      }
    }
    if (input == 'a') {
      armed = true;
      m_grip.write(pos_stop_grip);
      wanted_hauteur = pos_stop_vis;
      instant_pos_bras(pos_stop_bras);
    }
    if (input == 'd') {
      armed = false;
      actual_prog = 0;
    }
    if (input == 'P') {
      actual_prog = Serial.parseInt();
      phase = 0;
    }
    Serial.println(input);
  }

  if (armed) {
    exec_prog();
    exec_moteur();
  }

  if (wanted_hauteur > hauteur_max) {
    wanted_hauteur = hauteur_max;
  }

  if (wanted_hauteur < 0) {
    wanted_hauteur = 0;
  }

  if (digitalRead(pin_button)) {
    hauteur = hauteur_max;
  }
}
