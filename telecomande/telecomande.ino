#include <EEPROM.h>
#include "charactres_LM.h"
#include "Led_Matrix.h"


#define palier_0 150 // pour 1000       le palier a partir du quel les moteurs s'arréte
#define palier_sprint_y 75  // pour 1000       le palier a partir du quel les moteurs s'arréte
#define palier_sprint_autres 200 // pour 1000       le palier a partir du quel les moteurs s'arréte
#define sprint_speed 1000
#define max_speed 1000 // nb de steps    vitesse de rotation maximale des moteurs
#define rotation_ratio 0.25 // pour 1    ratio pour la vitesse de rotation
#define delay_recheck_analog 100 // ms          temps entre chauque réactualisation
#define ratio_de_recherche 0.9
#define acurate 1000


//pins systemes externes
#define pin_x A0
#define pin_y A1
#define pin_z A2
#define pin_grip A13
#define pin_hauteur A14
#define pin_bras A15
#define pin_moin_plateau 39
#define pin_plus_plateau 37
#define pin_moin_points 12
#define pin_plus_points 11
#define pin_prog_recup_pot 15
#define pin_prog_recup_plante 14
#define pin_prog_deposer_pot 17
#define pin_prog_deposer_pot_jardiniere 16
#define pin_latch_desarmement 5
#define pin_armement 8
#define pin_couleur 45
#define pin_start_game 49
#define pin_rouge 23
#define pin_blanc 25
#define pin_bleu 27
#define pin_jaune 29
#define pin_noir  31
#define pin_orange 33

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
 
//définition des constantes
#define deg_to_rad (PI/180)    //valeur de conversion degres radian    degres * deg_to_rad = radians

const int pince_input[] =               {pin_grip, pin_hauteur, pin_bras};
const String pice_send_index[] =        {"G", "v", "b"};
float last_pince_value[] =              {0, 0, 0};
const float max_pince_value[] =         {80, 126, 115};
const float min_pince_value[] =         {20, 0, 40};
const float multiplier_pince_value[] =  {1, 0.125, 1};
const int pince_len = sizeof(pince_input)/sizeof(pince_input[0]);
float pince_calcule;
boolean pince_mode_auto = true;
boolean pince_states_auto_button = true;

const int input[] = {pin_prog_recup_pot, pin_prog_recup_plante, pin_prog_deposer_pot, pin_prog_deposer_pot_jardiniere, pin_blanc, pin_bleu , pin_jaune, pin_noir, pin_orange};
const String input_send_msg[] = {"P1","P2","P3","P4","","","","","","P0"};
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
uint8_t pins_all_bras[] = {pin_grip,pin_hauteur,pin_bras};
int last_all_bras[] = {0,0,0};
bool state_armement = true;
unsigned long start_time = millis();
unsigned long actual_party_time = 0;
bool started = false;
bool start_sent = false;
unsigned long last_update = millis();

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

void write_points(int points){
   M1.i_Write(chiffres[points%10]);
   if(int(points/10)%10!=0){
    M2.i_Write(chiffres[int(points/10)%10]);
   }else{
    M2.i_Write(nothing);
   }
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
  if(x < palier_0 and x > (0-palier_0)){
    x=0;
  }else{
    x = map(x,palier_0,1000,0,1000);
  }
  if(y < palier_0 and y > (0-palier_0)){
    y=0;
  }else{
    y = map(y,palier_0,1000,0,1000);
  }
  if(z>1000){
    z = 1000;
  }
  if(z < -1000){
    z = -1000;
  }
  if(z < palier_0 and z > (0-palier_0)){
    z=0;
  }else{
    z = map(z,palier_0,1000,0,1000);
  }
  angle = XY_Move_To_Angle(x,y); 
  dist = XY_Move_To_Dist(x,y);
  r1 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*30)),-1000,1000,0 - acurate,acurate)*max_speed/acurate;
  r2 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*150)),-1000,1000,0 - acurate,acurate)*max_speed/acurate;
  r3 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*270)),-1000,1000,0 - acurate,acurate)*max_speed/acurate;

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
  for(int i = 0; i < pince_len; i++){
    pince_calcule = map(analogRead(pince_input[i]),0,1023,min_pince_value[i],max_pince_value[i])*multiplier_pince_value[i];
    if(pince_calcule != last_pince_value[i]){
      last_pince_value[i] = pince_calcule;
      Serial1.println(pice_send_index[i]+String(pince_calcule));
      if(pince_mode_auto){
        for(int j = 0; j < 3; j++){
          Serial1.println(pice_send_index[j]+String(last_pince_value[j]));
        }
        pince_mode_auto = false;
      }
    }
  }
}


//fonctions d'executions
 
//definision des donnée a execution unique 
void setup() {
  // ouverture du port serie pour la comunication et le debug
  pinMode(pin_moin_plateau,INPUT_PULLUP);
  pinMode(pin_plus_plateau,INPUT_PULLUP);
  pinMode(pin_moin_points,INPUT_PULLUP);
  pinMode(pin_plus_points,INPUT_PULLUP);
  for(int i = 0; i < sizeof(input)/sizeof(input[0]); i++){
    pinMode(input[i],INPUT_PULLUP);
  }
  Serial1.begin(9600);
  pinMode(pin_latch_desarmement ,INPUT_PULLUP);
  pinMode(pin_armement,INPUT_PULLUP);
  pinMode(pin_couleur,INPUT_PULLUP);
  pinMode(pin_start_game,INPUT_PULLUP);
}

//boucle principale
void loop() {

  if(digitalRead(pin_orange)){
    if(pince_states_auto_button){
      pince_states_auto_button = false;
      pince_mode_auto = true;
    }
  }else{
    pince_states_auto_button = true;
  }
  
  if(millis() - last_update > delay_recheck_analog){
    analogique_prossesing();
    last_update = millis();
  }
  
  if(digitalRead(pin_start_game)){ 
    start_time = millis();
    started = true;
    Serial1.println("c"+String(int(digitalRead(pin_couleur))));
    start_sent = false;
  }

  if(started){
    actual_party_time = millis() - start_time;
    if(actual_party_time > 100000 and !start_sent){
      Serial1.println("S");
      start_sent = true;
    }
  }
  
  if(digitalRead(pin_latch_desarmement)){
    if(digitalRead(pin_armement)){
      if(state_armement){
        Serial1.println("a");
        state_armement = false;
      }
    }else{
      state_armement = true;
    }
  }else{
    Serial1.println("d");
  }
  
  if(digitalRead(pin_moin_plateau)){
    if(state_moin_plateau_ready){
      state_moin_plateau_ready = false;
      pos_plateau = pos_plateau - 1;
    }
  }else{
    state_moin_plateau_ready = true;
  }
  
  if(digitalRead(pin_plus_plateau)){
    if(state_plus_plateau_ready){
      state_plus_plateau_ready = false;
      pos_plateau = pos_plateau + 1;
    }
  }else{
    state_plus_plateau_ready = true;
  }
  
  if(digitalRead(pin_moin_points)and points > 0){
    if(state_moin_points_ready){
      state_moin_points_ready = false;
      points = points - 1;
    }
  }else{
    state_moin_points_ready = true;
  }
  
  if(digitalRead(pin_plus_points)){
    if(state_plus_points_ready){
      state_plus_points_ready = false;
      points = points + 1;
    }
  }else{
    state_plus_points_ready = true;
  }

  for(int i = 0; i < input_len; i++){
    if(digitalRead(input[i])and input_send_msg[i] != ""){
      if(states[i]){
        states[i] = false;
        Serial1.println(input_send_msg[i]);
      }
    }else{
      states[i] = true;
    }
  }
  
  if(Serial.available()>0){
    Serial_income = Serial.read();
    if(Serial_income=='c'){
      int type = Serial.parseInt();
      if(type==0){
        x_0 = analogRead(pin_x);
        y_0 = analogRead(pin_y);
        z_0 = analogRead(pin_z);
      }
      if(type==1){
        x_max = analogRead(pin_x);
      }
      if(type==2){
        y_max = analogRead(pin_y);
      }
      if(type==3){
        z_max = analogRead(pin_z);
      }
      if(type==4){
        x_min = 2 * x_0 - x_max;
        y_min = 2 * y_0 - y_max;
        z_min = 2 * z_0 - z_max;
        //ecriture x
        EEPROM.write(0, round(x_min/4));
        EEPROM.write(1, round(x_max/4));
        //ecriture y
        EEPROM.write(2, round(y_min/4));
        EEPROM.write(3, round(y_max/4));
        //ecriture z
        EEPROM.write(4, round(z_min/4));
        EEPROM.write(5, round(z_max/4));
      }
    }
  }
  if(last_pos_plateau != pos_plateau){
    last_pos_plateau = pos_plateau;
    Serial1.println('g'+String(pos_plateau));
  }
  if(last_points != points){
    last_points = points;
    Serial1.println('p'+String(points));
    write_points(points);
  }
}
