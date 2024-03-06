#include <EEPROM.h>

//parametres
#define acurate 1000 // pour 1000       la précision du joystick 
#define palier_0 1001 // pour 1000       le palier a partir du quel les moteurs s'arréte
#define max_speed 150 // nb de steps    vitesse de rotation maximale des moteurs
#define rotation_ratio 0.5 // pour 1    ratio pour la vitesse de rotation
#define delay_recheck 10 // ms         temps entre chauque réactualisation

//pins systemes externes
#define pin_x A0
#define pin_y A1
#define pin_z A2
#define pin_moin_plateau 2
#define pin_plus_plateau 3
#define pin_moin_points 4
#define pin_plus_points 5
#define pin_prog_recup_pot 6
#define pin_prog_recup_plante 7
#define pin_prog_deposer_pot 8
#define pin_desarmement 9
#define pin_armement 10
#define pin_couleur 11
#define pin_start_game 12
 
//définition des constantes
#define deg_to_rad (PI/180)    //valeur de conversion degres radian    degres * deg_to_rad = radians

const int input[5] = {pin_prog_recup_pot, pin_prog_recup_plante, pin_prog_deposer_pot, pin_desarmement, pin_armement};
const String input_send_msg[5] = {"P1","P2","P3","d","a"};

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
char serial_income; //  arivée de données venant du port serie le port serie
bool updating = false; // autorise l'envoie de la trame de mise a jours des vitesse moteur
bool state_moin_plateau_ready = true;
bool state_plus_plateau_ready = true;
bool state_moin_points_ready = true;
bool state_plus_points_ready = true;
bool states[5] = {true,true,true,true,true};

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

//transforme les données d'un mouvement polaire definit par angle et dist, il utilise aussi les données de rotation du veicule et le paramétre d'angle de la roue pour calculer la vitesse que la roue doit avoir.
float Wheel_Speed(float angle,float dist,float rotation,float wheel_angle){
  float wheel_speed = dist*cos(angle-wheel_angle)+(rotation*rotation_ratio);
  return wheel_speed;
}


//fonctions d'executions
 
//definision des donnée a execution unique 
void setup() {
  // ouverture du port serie pour la comunication et le debug
  pinMode(pin_moin_plateau,INPUT);
  pinMode(pin_plus_plateau,INPUT);
  pinMode(pin_moin_points,INPUT);
  pinMode(pin_plus_points,INPUT);
  for(int i = 0; i < sizeof(input)/sizeof(input[0]); i++){
    pinMode(input[i],INPUT);
  }
  Serial.begin(9600);
  Serial.println("a");
}

//boucle principale
void loop() {
  
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

  for(int i = 0; i < sizeof(input)/sizeof(input[0]); i++){
    if(digitalRead(input[i])){
      if(states[i]){
        states[i] = false;
        Serial.println(input_send_msg[i]);
      }
    }else{
      states[i] = true;
    }
  }
  
  if(Serial.available()>0){
    serial_income = Serial.read();
    if(serial_income=='c'){
      int type = Serial.parseInt();
      if(type==0){
        x_0 = analogRead(pin_x);
        y_0 = analogRead(pin_y);
        z_0 = analogRead(pin_z);
      }
      if(type==1){
        x_max = analogRead(pin_x);
        z_max = analogRead(pin_z);
      }
      if(type==2){
        y_max = analogRead(pin_y);
      }
      if(type==3){
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
  x = map(analogRead(pin_x),x_min,x_max,-1000,1000);
  y = map(analogRead(pin_y),y_min,y_max,-1000,1000);
  z = map(analogRead(pin_z),z_min,z_max,-1000,1000);
  if(x < palier_0 and x > (0-palier_0)){
    x=0;
  }
  if(y < palier_0 and y > (0-palier_0)){
    y=0;
  }
  if(z < palier_0 and z > (0-palier_0)){
    z=0;
  }
  angle = XY_Move_To_Angle(x,y); 
  dist = XY_Move_To_Dist(x,y);
  r1 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*30)),-1000,1000,0 - acurate,acurate)*max_speed/acurate;
  r2 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*150)),-1000,1000,0 - acurate,acurate)*max_speed/acurate;
  r3 =  map(Wheel_Speed(angle,dist,z,(deg_to_rad*270)),-1000,1000,0 - acurate,acurate)*max_speed/acurate;
  if(last_r1 != r1){
    last_r1 = r1;
    Serial.println('1'+String(r1));
    updating = true;
  }
  if(last_r2 != r2){
    last_r2 = r2;
    Serial.println('2'+String(r2));
    updating = true;
  }
  if(last_r3 != r3){
    last_r3 = r3;
    Serial.println('3'+String(r3));
    updating = true;
  }
  if(updating == true){
    updating = false;
    Serial.println('u');
  }
  if(last_pos_plateau != pos_plateau){
    last_pos_plateau = pos_plateau;
    Serial.println('g'+String(pos_plateau));
  }
  if(last_points != points){
    last_points = points;
    Serial.println('p'+String(points));
  }
  delay(delay_recheck);
}
