//intégration
#include "charactres_LM.h"
#include "Led_Matrix.h"
#include <AccelStepper.h>

//parametres
#define motor_limit_speed 65000

//constantes
const char values[]{'G','v','b','a','d','P'};
const char values2[]{'S','c'};

// definition des matrices led et leurs pins
   // matrice 1 
#define M1_DIN_pin 53
#define M1_CS_pin 51
#define M1_CLK_pin 49
   // matrice 2 
#define M2_DIN_pin 52
#define M2_CS_pin 50
#define M2_CLK_pin 48

Led_Matrix M1(M1_DIN_pin, M1_CS_pin, M1_CLK_pin);
Led_Matrix M2(M2_DIN_pin, M2_CS_pin, M2_CLK_pin);

//definition des  stepper et leurs pins
#define ENABLE_PIN         8
#define STEPPER1_STEP_PIN  2
#define STEPPER1_DIR_PIN   5
#define STEPPER2_STEP_PIN  3
#define STEPPER2_DIR_PIN   6
#define STEPPER3_STEP_PIN  4
#define STEPPER3_DIR_PIN   7
#define P_STEP_PIN         12 
#define P_DIR_PIN          13

AccelStepper stepper1(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);
AccelStepper stepper3(AccelStepper::DRIVER, STEPPER3_STEP_PIN, STEPPER3_DIR_PIN);
AccelStepper stepperP(AccelStepper::DRIVER, P_STEP_PIN , P_DIR_PIN );





//definition des variables
char serial_income; //  arivée de données venant du port serie le port serie
float r1;    //         valeur de vitesse du moteur de la roue 1
float r2;    //         valeur de vitesse du moteur de la roue 2
float r3;    //         valeur de vitesse du moteur de la roue 3
int points = 0; //      nombre de points dans la partie
bool armed;  //         armement des stepper
int len;
bool founded;
unsigned long animation_start = millis();
unsigned long accel_time = millis();
int animation_fps = 6 ;
float curSpeed1;
float newSpeed1;
float curSpeed2;
float newSpeed2;
float curSpeed3;
float newSpeed3;
float s;
unsigned long lastT, t;
float acc = 250;

void write_points(int points){
   M1.Write(chiffres[points%10]);
   if(int(points/10)%10!=0){
    M2.Write(chiffres[int(points/10)%10]);
   }else{
    M2.Write(nothing);
   }
}

void update_motors_speed(){
  newSpeed1 = r1;
  newSpeed2 = r2;
  newSpeed3 = r3;
}

void run_motors_speed(){
  t=millis();
  if ((t-lastT) > 1)
  {
    if (newSpeed1 > curSpeed1)
    {
      curSpeed1+=0.001*acc*(t-lastT);
      if (curSpeed1 > newSpeed1)
        curSpeed1 = newSpeed1;
    }
    else if (newSpeed1 < curSpeed1)
    {
      curSpeed1-=0.01*acc*(t-lastT);
      if (curSpeed1 < newSpeed1)
        curSpeed1 = newSpeed1;
    }

    if (newSpeed2 > curSpeed2)
    {
      curSpeed2+=0.01*acc;
      if (curSpeed2 > newSpeed2)
        curSpeed2 = newSpeed2;
    }
    else if (newSpeed2 < curSpeed2)
    {
      curSpeed2-=0.01*acc;
      if (curSpeed2 < newSpeed2)
        curSpeed2 = newSpeed2;
    }
    
    if (newSpeed3 > curSpeed3)
    {
      curSpeed3+=0.01*acc;
      if (curSpeed3 > newSpeed3)
        curSpeed3 = newSpeed3;
    }
    else if (newSpeed3 < curSpeed3)
    {
      curSpeed3-=0.01*acc;
      if (curSpeed3 < newSpeed3)
        curSpeed3 = newSpeed3;
    }
    stepper1.setSpeed(curSpeed1);
    stepper2.setSpeed(curSpeed2);
    stepper3.setSpeed(curSpeed3);
    lastT=t;
  }
  stepper1.runSpeed();
  stepper2.runSpeed();
  stepper3.runSpeed();
}

//fonctions d'executions
 
//definision des donnée a execution unique 
void setup() {
  // ouverture du port serie pour la comunication et le debug
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
    
  //gestion des stepper
  pinMode (ENABLE_PIN,OUTPUT);
  stepper1.setMaxSpeed(65000);
  stepper1.setSpeed(0);  
  stepper2.setMaxSpeed(65000);
  stepper2.setSpeed(0);  
  stepper3.setMaxSpeed(65000);
  stepper3.setSpeed(0);  
  curSpeed1=0;
  newSpeed1=0;
  curSpeed2=0;
  newSpeed2=0;
  curSpeed3=0;
  newSpeed3=0;
  lastT = millis();
  stepperP.enableOutputs(); 
  
  digitalWrite (ENABLE_PIN,HIGH);
  armed = false;
  r1 = 0;
  r2 = 0;
  r3 = 0;
}

//boucle principale
void loop() {
  if(Serial.available()>0){
    serial_income = Serial.read();
    if(serial_income =='d'){ // desarmer stepper 
      digitalWrite (ENABLE_PIN,HIGH);
      armed = false;
      r1 = 0;
      r2 = 0;
      r3 = 0;
      curSpeed1=0;
      newSpeed1=0;
      curSpeed2=0;
      newSpeed2=0;
      curSpeed3=0;
      newSpeed3=0;
      stepper1.setSpeed(0);
      stepper2.setSpeed(0);
      stepper3.setSpeed(0);
    }
    if(serial_income =='a'){ // armer les stepper
      digitalWrite (ENABLE_PIN,LOW);
      armed = true;
    }
    if(serial_income =='A'){ // reboot l'animation 
      animation_start = millis();
    }
    if(serial_income =='p'){
      points = Serial.parseInt();
    }
    if(serial_income =='1'){
      r1 = Serial.parseInt();
    }
    if(serial_income =='2'){
      r2 = Serial.parseInt();
    }
    if(serial_income =='3'){
      r3 = Serial.parseInt();
    }
    if(serial_income =='g'){
      stepperP.moveTo(int(4000*Serial.parseInt()));
    }
    if(serial_income =='u'){
      update_motors_speed();
    }
    len = sizeof(values)/sizeof(values[0]);
    founded = false;
    for(int i = 0; i < len and !founded ; i++){
      if(values[i] == serial_income){
        founded = true;
        Serial1.println(values[i]+String(Serial.parseInt()));
      }
    }
    len = sizeof(values2)/sizeof(values2[0]);
    founded = false;
    for(int i = 0; i < len and !founded ; i++){
      if(values2[i] == serial_income){
        founded = true;
        Serial2.println(values[i]+String(Serial.parseInt()));
      }
    }
  }
  if(armed==true){
    run_motors_speed();
    stepperP.run();    
  }
  if (int((millis()-animation_start)*animation_fps/1000 < 18)){
    M1.Write(start_animation_1[int((millis()-animation_start)*animation_fps/1000)]);
    M2.Write(start_animation_2[int((millis()-animation_start)*animation_fps/1000)]);
  }else{
    write_points(points);
  }
}
