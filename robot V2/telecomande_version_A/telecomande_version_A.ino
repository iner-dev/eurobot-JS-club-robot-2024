#include <EEPROM.h>
#include "charactres_LM.h"
#include "Led_Matrix.h"
#include "codeur.h"

#define DEBUG

#define palier_0 500 // pour 1000       le palier a partir du quel les moteurs s'arréte
#define palier_0_z 400
#define palier_sprint_y 1000  // pour 1000       le palier a partir du quel les moteurs s'arréte
#define palier_sprint_autres 600 // pour 1000       le palier a partir du quel les moteurs s'arréte
#define sprint_speed 1000
#define max_speed 1000 // nb de steps    vitesse de rotation maximale des moteurs
#define min_speed 200 // nb de steps    vitesse de rotation maximale des moteurs
#define rotation_ratio 0.125 // pour 1    ratio pour la vitesse de rotation
#define delay_recheck_analog 50 // ms          temps entre chauque réactualisation
#define ratio_de_recherche 1.1
#define acurate 100
#define total_time 99000 //ms      temps de la partie
#define pami_start_time 88500 // ms temps de demarage des PAMI


//pins systemes externes
#define pin_x A12
#define pin_y A11
#define pin_z A10
#define pin_grip A8
#define pin_deplacement A9
#define pin_moin_plateau 39
#define pin_plus_plateau 37
#define pin_moin_points 12
#define pin_plus_points 11
#define pin_prog_recup_pot 22 //             croix     blanc
#define pin_prog_recup_plante 24  //         carré     noir
#define pin_prog_deposer_pot 26 //           rond      bleu
#define pin_prog_deposer_pot_jardiniere 28// triangle  rien
#define pin_latch_desarmement 5
#define pin_armement 8
#define pin_couleur 45
#define pin_start_game 47
#define pin_rouge 23
#define pin_blanc 25
#define pin_bleu 27
#define pin_jaune 29
#define pin_noir  31
#define pin_orange 33
#define pin_serie1 13

// definition des matrices led et leurs pins
   // matrice 1 
#define M1_DIN_pin 52
#define M1_CS_pin 50
#define M1_CLK_pin 48
   // matrice 2 
#define M2_DIN_pin 53
#define M2_CS_pin 51
#define M2_CLK_pin 49

Led_Matrix M1(M1_DIN_pin, M1_CS_pin, M1_CLK_pin);
Led_Matrix M2(M2_DIN_pin, M2_CS_pin, M2_CLK_pin);

Codeur codeur_1(2, 32, false, true);
 
//définition des constantes
#define deg_to_rad (PI/180)    //valeur de conversion degres radian    degres * deg_to_rad = radians

const int pince_input[] =               {pin_grip, pin_deplacement};
const char pince_send_index[] =        {'G', 'm'};
const float max_pince_value[] =         {20, 50};
const float min_pince_value[] =         {5, 50};
const float multiplier_pince_value[] =  {4, 1};
const float pince_switch_level = 20; // en % 
const float pince_switch_level_lowtime = 1; // en % 
const int pince_switch_time_lowtime = 50000; // en ms 
unsigned long lowtime_start[] = {0,0};
float last_pince_value[] = {0, 0};
unsigned long actual_time = 0;
unsigned long actual_calculated_time = 0;
const int pince_len = sizeof(pince_input)/sizeof(pince_input[0]);
float pince_calcule;
boolean pince_mode_manuel = true;
boolean pince_states_auto_button = true;
float pince_switch_level_list[] = {0,0};
float pince_switch_level_list_lowtime[] = {0,0};
boolean resending = false;
int act_speed = max_speed;
bool state_bleu = true;
bool state_blanc = true;


const int input[] = {pin_prog_recup_pot, pin_prog_recup_plante, pin_prog_deposer_pot, pin_prog_deposer_pot_jardiniere, pin_blanc, pin_bleu , pin_jaune, pin_noir, pin_orange};
const String input_send_msg[] = {"P1","P2","P3","P4","","","h","","P0"};
bool states[] = {true,true,true,true,true,true,true,true,true,true};
const int input_len = sizeof(input)/sizeof(input[0]);

//definition des variables
float x = 0; //         valeur x du joystick
float y = 0; //         valeur y du joystick
float z = 0; //         valeur z du joystick
float r1;    //         valeur de vitesse du moteur de la roue 1
float last_r1; //       valeur précédante de vitesse du moteur de la roue 1
float r2;    //         valeur de vitesse du moteur de la roue 2
float last_r2; //       valeur précédante de vitesse du moteur de la roue 2
float r3;    //         valeur de vitesse du moteur de la roue 3
float last_r3; //       valeur précédante de vitesse du moteur de la roue 3
float angle; //         angle de mouvement sur un plan polaire
float dist;  //         distance de mouvement sur un plan polaire
    // valeurs de calibrage du joystick
int x_min = EEPROM.read(0)*4; 
int x_max = EEPROM.read(1)*4;
int x_0;
int y_min = EEPROM.read(2)*4;
int y_max = EEPROM.read(3)*4;
int y_0;
int z_min = EEPROM.read(4)*4;
int z_max = EEPROM.read(5)*4;
int z_0;
int last_points = 0;
int points = 0;
int last_pos_plateau = 0;
int pos_plateau = 0;
char Serial_income; //  arivée de données venant du port serie le port serie
bool updating = false; // autorise l'envoie de la trame de mise a jours des vitesse moteur
bool state_moin_plateau_ready = true;
bool state_plus_plateau_ready = true;
bool state_moin_points_ready = true;
bool state_plus_points_ready = true;
int last_all_bras[] = {0,0,0};
bool state_armement = true;
unsigned long start_time = millis();
unsigned long actual_party_time = 0;
bool started = false;
bool start_sent = false;
bool start_state = false;
bool state_noir = false;
unsigned long last_update = millis();
int screen_mode = 0;
int party_time = 0;
int last_party_time = 0;
const int analog_input[] = {pin_x, pin_y, pin_z, pin_grip, pin_deplacement};
int analog_auto_set = -1;
bool janotus = true;


//definitions des fonctions

//transforme une valeur vectorielle X Y en un angle en radian d'un mouvement sur un plan polaire
float XY_Move_To_Angle(float x,float y){
  float angle;
  if(y==0){
    if(0>x){
      angle = 3*PI/2;
    }else{
      angle = PI/2;
    }
  }else{
    if(y>0){
      angle = atan(x/y);
    }else{
      angle = PI + atan(x/y);
    }
  }
  return angle;
}

//transforme une valeur vectorielle X Y en un distance d'un mouvement sur un plan polaire 
float XY_Move_To_Dist(float x,float y){
  float dist = sqrt(sq(x)+sq(y));
  if(dist>1000){
    dist = 1000;
  }
  return dist;
}

void write_val(int points){
   M1.i_Write(lettres[points%10 +26]);
   if(int(points/10)%10!=0){
    M2.i_Write(lettres[int(points/10)%10 +26]);
   }else{
    M2.i_Write(nothing);
   }
}

void write_matrix(String val){
  M1.i_Write(char_To_Askii_Art(val[1]));
  M2.i_Write(char_To_Askii_Art(val[0]));
}

//transforme les données d'un mouvement polaire definit par angle et dist, il utilise aussi les données de rotation du veicule et le paramétre d'angle de la roue pour calculer la vitesse que la roue doit avoir.
float Wheel_Speed(float angle,float dist,float rotation,float wheel_angle){
  float wheel_speed = dist*cos(angle-wheel_angle)+(rotation*rotation_ratio);
  return wheel_speed;
}

void analogique_prossesing(){
  x = map(analogRead(pin_x),x_min,x_max,-1000,1000)* ratio_de_recherche;
  y = map(analogRead(pin_y),y_min,y_max,-1000,1000)* ratio_de_recherche;
  z = map(analogRead(pin_z),z_min,z_max,-1000,1000)* ratio_de_recherche;
  if (z<-1500)
    z=0;
  else if (z > 1300)
    z=0;
  angle = XY_Move_To_Angle(x,y); 
  dist = XY_Move_To_Dist(x,y);
  if(dist < palier_0 and dist > (0-palier_0)){
    dist=0;
  }else{
    dist = map(dist,palier_0,1000,0,1000);
  }
  if(z < palier_0_z and z > (0-palier_0_z)){
    z=0;
  }else{
    z = map(z,palier_0,1000,0,1000);
  }
  r1 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*30)),-1000,1000,0 - acurate,acurate)*act_speed/acurate;
  r2 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*150)),-1000,1000,0 - acurate,acurate)*act_speed/acurate;
  r3 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*270)),-1000,1000,0 - acurate,acurate)*act_speed/acurate;
  
  if(y > (palier_sprint_y) and x < palier_sprint_autres and z < palier_sprint_autres and x > (0-palier_sprint_autres) and z > (0-palier_sprint_autres)){
    r1 = sprint_speed;
    r2 = 0 - sprint_speed;
    r3 = 0;
  }
  if(last_r1 != r1){
    last_r1 = r1;
    Serial1.println('1'+String(r1));
    updating = true;
  }
  if(last_r2 != r2){
    last_r2 = r2;
    Serial1.println('2'+String(r2));
    updating = true;
  }
  if(last_r3 != r3){
    last_r3 = r3;
    Serial1.println('3'+String(r3));
    updating = true;
  }
  if(updating == true){
    updating = false;
    Serial1.println('u');
  }
  actual_time = millis();
  actual_calculated_time = actual_time - pince_switch_time_lowtime;
  for(int i = 0; i < pince_len; i++){
    pince_calcule = map(analogRead(pince_input[i]),0,1023,min_pince_value[i],max_pince_value[i])*multiplier_pince_value[i];
    if(pince_mode_manuel){
      if(abs(last_pince_value[i] - pince_calcule)>pince_switch_level_list_lowtime[i]){
        lowtime_start[i] = actual_time ;
        last_pince_value[i] = pince_calcule;
        Serial1.println(pince_send_index[i]+String(pince_calcule));
      }else{
        if(actual_calculated_time <= lowtime_start[i] and last_pince_value[i] != pince_calcule){
          last_pince_value[i] = pince_calcule;
          Serial1.println(pince_send_index[i]+String(pince_calcule));
        }
      }
    }else{ 
      if(abs(last_pince_value[i] - pince_calcule)>pince_switch_level_list[i]){
        for(int j = 0; j < 3; j++){
          Serial1.println(pince_send_index[j]+String(last_pince_value[j]));
        }
        pince_mode_manuel = true;
      }
    }
  }
}

void screen_actualise(bool force = false){
  if(screen_mode == 1){
    write_val(codeur_1.getPosition());
  }
  if(screen_mode == 2){
    party_time = (total_time-(millis()-start_time))/1000;
    if(last_party_time != party_time or force){
      last_party_time = party_time;
      if(party_time >= 0){
        write_val(party_time);
      }else{
        write_matrix("FI");
      }
    }
  }
}

void actualise_debug(){
  if(Serial.available()>0){
    Serial_income = Serial.read();
    if(resending){
      if(Serial_income == 'z'){
        resending = false;
      }else{
        Serial1.println(Serial_income + String(Serial.parseInt()));
      }
    }else{
      if(Serial_income=='J'){
        janotus = true;
      }
      if(Serial_income=='j'){                                                           
        janotus = false;
        x = 0;
        y = 0;
        z = 0;
      }
      if(Serial_income=='R'){
        analog_auto_set = Serial.parseInt();
      }
      if(Serial_income=='c'){
        int type = Serial.parseInt();
        if(type==0){
          x_0 = analogRead(pin_x);
          y_0 = analogRead(pin_y);
          z_0 = analogRead(pin_z);
          Serial.println("--------------------------------------------------------");
          Serial.println("calibrating protocol started");
          Serial.println("");
          Serial.println("you calibrate the 0 of all axes");
          Serial.println("the next step is calibrating the X axes");
          Serial.println("for that hold stick on left and enter command c1");
        }
        if(type==1){
          x_max = analogRead(pin_x);
          x_min = 2 * x_0 - x_max;
          EEPROM.write(0, round(x_min/4));
          EEPROM.write(1, round(x_max/4));
          Serial.println("");
          Serial.println("x calibration done");
          Serial.println("the next step is calibrating the Y axes");
          Serial.println("for that hold stick backward and enter command c2");
        }
        if(type==2){
          y_min = analogRead(pin_y);
          y_max = 2 * y_0 - y_min;
          EEPROM.write(2, round(y_min/4));
          EEPROM.write(3, round(y_max/4));
          Serial.println("");
          Serial.println("Y calibration done");
          Serial.println("the final step is calibrating the Z axes");
          Serial.println("for that turn the stick Counterclockwise and enter command c3");
        }
        if(type==3){
          z_max = analogRead(pin_z);
          z_min = 2 * z_0 - z_max;
          EEPROM.write(4, round(z_min/4));
          EEPROM.write(5, round(z_max/4));
          Serial.println("");
          Serial.println("Z calibration done");
          Serial.println("all calibration done");
        }
      }
      if(Serial_income=='x'){
        Serial.println("--------------------------------------------------------");
        Serial.println("x avant "+String(x));
        x = Serial.parseInt();
        Serial.println("x apres "+String(x));
      }
      if(Serial_income=='y'){
        Serial.println("--------------------------------------------------------");
        Serial.println("y avant "+String(y));
        y = Serial.parseInt();
        Serial.println("y apres "+String(y));
      }
      if(Serial_income=='z'){
        Serial.println("--------------------------------------------------------");
        Serial.println("z avant "+String(z));
        z = Serial.parseInt();
        Serial.println("z apres "+String(z));
      }
      for(int i = 0; 3 > i; i++){
        if(pince_send_index[i] == Serial_income){
          Serial.println("--------------------------------------------------------");
          Serial.println(String(pince_send_index[i]) + " est egal à " + String(map(analogRead(pince_input[i]),0,1023,min_pince_value[i],max_pince_value[i])*multiplier_pince_value[i]));
        }
      }
      if(Serial_income=='r'){
        Serial.println("--------------------------------------------------------");
        resending = true;
      }
    }
  }
  if(analog_auto_set >= 0 and analog_auto_set <= 5){
    Serial.println(analogRead(analog_input[analog_auto_set]));
  }
}





//fonctions d'executions
 
//definision des donnée a execution unique 
void setup() {
  for(int i = 0; i < pince_len; i++){
    pince_switch_level_list[i] = abs(max_pince_value[i] - min_pince_value[i])*pince_switch_level/100;
  }
  for(int i = 0; i < pince_len; i++){
    pince_switch_level_list_lowtime[i] = abs(max_pince_value[i] - min_pince_value[i])*pince_switch_level_lowtime/100;
  }
  // ouverture du port serie pour la comunication et le debug
  pinMode(pin_moin_plateau,INPUT_PULLUP);
  pinMode(pin_plus_plateau,INPUT_PULLUP);
  pinMode(pin_moin_points,INPUT_PULLUP);
  pinMode(pin_plus_points,INPUT_PULLUP);
  for(int i = 0; i < sizeof(input)/sizeof(input[0]); i++){
    pinMode(input[i],INPUT_PULLUP);
  }
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(pin_latch_desarmement ,INPUT_PULLUP);
  pinMode(pin_armement,INPUT_PULLUP);
  pinMode(pin_couleur,INPUT_PULLUP);
  pinMode(pin_start_game,INPUT_PULLUP);
  pinMode(pin_serie1,OUTPUT);
  digitalWrite(pin_serie1,HIGH);
  write_matrix("DA");
  codeur_1.init();
}





//boucle principale
void loop() { 
  actualise_debug();
  if(pince_mode_manuel){
    if(!digitalRead(pin_noir) or !digitalRead(pin_prog_recup_pot) or !digitalRead(pin_prog_recup_plante) or !digitalRead(pin_prog_deposer_pot) or !digitalRead(pin_prog_deposer_pot_jardiniere)){
      if(pince_states_auto_button){
        pince_states_auto_button = false;
        pince_mode_manuel = false;
      }
    }else{
      pince_states_auto_button = true;
    }
  }
  
  if(millis() - last_update > delay_recheck_analog){
    analogique_prossesing();
    last_update = millis();
  }

  if(!digitalRead(pin_start_game)){ 
    if(start_state){
      if(digitalRead(pin_latch_desarmement)){
        start_state = false;
        start_time = millis();
        started = true;
        Serial1.println("c"+String(int(!digitalRead(pin_couleur))));
        start_sent = false;
        Serial1.println("a");
        screen_mode = 1;
        write_val(points);
      }else{
        started = false;
      }
    }
  }else{
    if(!start_state){
      start_state = true;
    }
  }

  if(started){
    actual_party_time = millis() - start_time;
    if(actual_party_time > pami_start_time and !start_sent and digitalRead(pin_latch_desarmement)){
      Serial1.println("S");
      start_sent = true;
    }
    if(actual_party_time > total_time){
      started = false;
      Serial1.println("d");
      screen_mode = 0;
      write_matrix("FI");
    }
  }
  
  if(digitalRead(pin_latch_desarmement)){
    if(!digitalRead(pin_armement)){
      if(state_armement){
        Serial1.println("a");
        screen_mode = 1;
        screen_actualise();
        state_armement = false;
      }
    }else{
      state_armement = true;
    }
  }else{
    Serial1.println("d");
    screen_mode = 0;
    write_matrix("DA");
  }
  
  if(!digitalRead(pin_moin_plateau)){
    if(state_moin_plateau_ready){
      state_moin_plateau_ready = false;
      pos_plateau = pos_plateau - 1;
    }
  }else{
    state_moin_plateau_ready = true;
  }
  
  if(!digitalRead(pin_plus_plateau)){
    if(state_plus_plateau_ready){
      state_plus_plateau_ready = false;
      pos_plateau = pos_plateau + 1;
    }
  }else{
    state_plus_plateau_ready = true;
  }
  
  if(!digitalRead(pin_moin_points)and codeur_1.getPosition() > 0){
    if(state_moin_points_ready){
      state_moin_points_ready = false;
      codeur_1.add_Position(-1);
    }
  }else{
    state_moin_points_ready = true;
  }
  
  if(!digitalRead(pin_plus_points)){
    if(state_plus_points_ready){
      state_plus_points_ready = false;
      codeur_1.add_Position(1);
    }
  }else{
    state_plus_points_ready = true;
  }

  for(int i = 0; i < input_len; i++){
    if(!digitalRead(input[i])and input_send_msg[i] != ""){
      if(states[i]){
        states[i] = false;
        Serial1.println(input_send_msg[i]);
      }
    }else{
      states[i] = true;
    }
  }

  if(!digitalRead(pin_noir)){
    if(state_noir){
      state_noir = false;
      if(screen_mode == 1){
        screen_mode = 2;
      }else{
        if(screen_mode == 2){
          screen_mode = 1;
        }
      }
      screen_actualise(true);
    }
  }else{
    state_noir = true;
  }

  if(!digitalRead(pin_bleu)){
    if(state_bleu){
      state_bleu = false;
      act_speed = max_speed;
    }
  }else{
    state_bleu = true;
  }

  if(!digitalRead(pin_blanc)){
    if(state_blanc){
      state_blanc = false;
      act_speed = min_speed;
    }
  }else{
    state_blanc = true;
  }
  if(last_pos_plateau != pos_plateau){
    last_pos_plateau = pos_plateau;
    Serial1.println('g'+String(pos_plateau));
  }
  if(last_points != codeur_1.getPosition()){
    last_points = codeur_1.getPosition();
    Serial1.println('p'+String(codeur_1.getPosition()));
    screen_actualise();
  }
  if(screen_mode == 2){
    screen_actualise();
  }
}
