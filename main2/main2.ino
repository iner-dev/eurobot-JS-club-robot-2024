//intégration
#include <AccelStepper.h>
#include "matrices_led.h"

//parametres
#define motor_limit_speed 200

//constantes
const char values[3]{'G','v','b'};


// definition des matrices led et leurs pins
   // matrice 1 
#define M1_DIN_pin 53
#define M1_CS_pin 51
#define M1_CLK_pin 49
   // matrice 2 
#define M2_DIN_pin 52
#define M2_CS_pin 50
#define M2_CLK_pin 48

// definition des modeles des chifres des paneaux leds
int nothing[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
int arrow[8] = {B00011000,B00111100,B01111110,B11111111,B00011000,B00011000,B00011000,B00011000};
int chifres[10][8] = {
  {
    B00000000, //0
    B01111110,
    B10000001,
    B10000001,
    B10000001,
    B10000001,
    B01111110,
    B00000000
  },
  {
    B00000000,  //1
    B00000000,
    B10000000,
    B10000001,
    B11111111,
    B10000000,
    B00000000,
    B00000000
  },
  {
    B00000000,  //2
    B01110000,
    B10001001,
    B10001001,
    B10001001,
    B10001001,
    B00000110,
    B00000000
  },
  {
    B00000000,  //3
    B10000001,
    B10001001,
    B10001001,
    B10001001,
    B10001001,
    B01110110,
    B00000000
  },
  {
    B00000000,  //4
    B00001111,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B11111111,
    B00000000
  },
  {
    B00000000,  //5
    B00000110,
    B10001001,
    B10001001,
    B10001001,
    B10001001,
    B01110000,
    B00000000
  },
  {
    B00000000,  //6
    B01110110,
    B10001001,
    B10001001,
    B10001001,
    B10001001,
    B01110000,
    B00000000
  },
  {
    B00000000,  //7
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B11111110,
    B00000000
  },
  {
    B00000000,  //8
    B01110110,
    B10001001,
    B10001001,
    B10001001,
    B10001001,
    B01110110,
    B00000000
  },
  {
    B00000000,  //9
    B00000110,
    B10001001,
    B10001001,
    B10001001,
    B10001001,
    B01110110,
    B00000000
  }
};

//definition des  stepper et leurs pins
#define ENABLE_PIN       8  

#define R1_STEP_PIN         2    // ex x
#define R1_DIR_PIN          5 

#define R2_STEP_PIN         3    // ex y
#define R2_DIR_PIN          6 

#define R3_STEP_PIN         4     // ex Z
#define R3_DIR_PIN          7

#define P_STEP_PIN         12     // ex A
#define P_DIR_PIN          13

AccelStepper stepperR1(AccelStepper::DRIVER, R1_STEP_PIN, R1_DIR_PIN);
AccelStepper stepperR2(AccelStepper::DRIVER, R2_STEP_PIN, R2_DIR_PIN);
AccelStepper stepperR3(AccelStepper::DRIVER, R3_STEP_PIN, R3_DIR_PIN);
AccelStepper stepperP(AccelStepper::DRIVER, P_STEP_PIN, P_DIR_PIN);

//definition des variables
char serial_income; //  arivée de données venant du port serie le port serie
float r1;    //         valeur de vitesse du moteur de la roue 1
float r2;    //         valeur de vitesse du moteur de la roue 2
float r3;    //         valeur de vitesse du moteur de la roue 3
int points = 0; //      nombre de points dans la partie
bool armed;  //         armement des stepper


void write_points(int points){
   write_matrix(chifres[points%10],M1_DIN_pin,M1_CS_pin,M1_CLK_pin);
   if(chifres[int(points/10)%10]!=0){
    write_matrix(chifres[int(points/10)%10],M2_DIN_pin,M2_CS_pin,M2_CLK_pin);
   }else{
    write_matrix(nothing,M2_DIN_pin,M2_CS_pin,M2_CLK_pin);
   }
}

void update_motors_speed(float r1, float r2, float r3){
  stepperR1.setSpeed(r1);
  stepperR2.setSpeed(r2);
  stepperR3.setSpeed(r3);
}

void run_motors_speed(){
  stepperR1.runSpeed();
  stepperR2.runSpeed();
  stepperR3.runSpeed();
}

//fonctions d'executions
 
//definision des donnée a execution unique 
void setup() {
  // ouverture du port serie pour la comunication et le debug
  Serial.begin(9600);
  Serial1.begin(9600);
    
  //gestion des stepper
  pinMode (ENABLE_PIN,OUTPUT);
  pinMode (R1_DIR_PIN,OUTPUT);
  pinMode (R1_STEP_PIN,OUTPUT);
  pinMode (R2_DIR_PIN,OUTPUT);
  pinMode (R2_STEP_PIN,OUTPUT);
  pinMode (R3_DIR_PIN,OUTPUT);
  pinMode (R3_STEP_PIN,OUTPUT);
  pinMode (P_DIR_PIN,OUTPUT);
  pinMode (P_STEP_PIN,OUTPUT);
  
  stepperR1.setPinsInverted(false, false, true); 
  stepperR1.setAcceleration(100.0);
  stepperR1.setMaxSpeed(motor_limit_speed);
    
  stepperR2.setMaxSpeed(motor_limit_speed);
  stepperR2.setAcceleration(100.0);
  stepperR2.setPinsInverted(false, false, true);      

  stepperR3.setMaxSpeed(motor_limit_speed);
  stepperR3.setAcceleration(100.0);
  stepperR3.setPinsInverted(false, false, true);      
  
  stepperP.setMaxSpeed(250.0);
  stepperP.setAcceleration(250.0);
  stepperP.setPinsInverted(false, false, true);      
  stepperP.setCurrentPosition(0);
  
  stepperR1.enableOutputs();  
  stepperR2.enableOutputs();
  stepperR3.enableOutputs(); 
  stepperP.enableOutputs(); 
  
  digitalWrite (ENABLE_PIN,HIGH);
  armed = false;
  r1 = 0;
  r2 = 0;
  r3 = 0;

  // activation des panneaux  leds
  pinMode(M1_CS_pin, OUTPUT);
  pinMode(M1_DIN_pin, OUTPUT);
  pinMode(M1_CLK_pin, OUTPUT);
  init_matrix(M1_DIN_pin,M1_CS_pin,M1_CLK_pin);
  
  pinMode(M2_CS_pin, OUTPUT);
  pinMode(M2_DIN_pin, OUTPUT);
  pinMode(M2_CLK_pin, OUTPUT);
  init_matrix(M2_DIN_pin,M2_CS_pin,M2_CLK_pin);

  write_points(points);
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
      update_motors_speed(r1,r2,r3);
    }
    if(serial_income =='a'){ // armer les stepper
      digitalWrite (ENABLE_PIN,LOW);
      armed = true;
    }
    if(serial_income =='p'){
      points = Serial.parseInt();
      write_points(points);
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
      stepperP.moveTo(int(250*Serial.parseInt()));
    }
    if(serial_income =='u'){
      update_motors_speed(r1,r2,r3);
    }
    for(int i = 0; i < sizeof(values)/sizeof(values[0]); i++){
      if(values[i] == serial_income){
        Serial1.println(values[i]+String(Serial.parseInt()));
      }
    }
  }
  if(armed==true){
    run_motors_speed();
    stepperP.run();    
  }
}
