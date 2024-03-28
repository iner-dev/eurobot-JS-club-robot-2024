
//parametres
#include <AccelStepper.h>

//definition des  stepper et leurs pins

#define R1_STEP_PIN         2    // ex x
#define R1_DIR_PIN          5 
#define R1_ENABLE_PIN       8  

#define R2_STEP_PIN         3    // ex y
#define R2_DIR_PIN          6 
#define R2_ENABLE_PIN       8

#define R3_STEP_PIN         4     // ex Z
#define R3_DIR_PIN          7
#define R3_ENABLE_PIN       8 

AccelStepper stepperR1(AccelStepper::DRIVER, R1_STEP_PIN, R1_DIR_PIN);
AccelStepper stepperR2(AccelStepper::DRIVER, R2_STEP_PIN, R2_DIR_PIN);
AccelStepper stepperR3(AccelStepper::DRIVER, R3_STEP_PIN, R3_DIR_PIN);

//valeur de conversion degres radian    degres * deg_to_rad = radians
#define deg_to_rad (PI/180)


//definition des variables
float x = 0; //         valeur x du joystick
float y = 0; //         valeur y du joystick
float z = 0; //         valeur z du joystick
char serial_income; //  arivée de données venant du port serie le port serie
float r1;    //         valeur de vitesse du moteur de la roue 1
float r2;    //         valeur de vitesse du moteur de la roue 2
float r3;    //         valeur de vitesse du moteur de la roue 3
float angle; //         angle de mouvement sur un plan polaire
float dist;  //         distance de mouvement sur un plan polaire
bool armed;  //         armement des stepper


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
  if(dist>200){
    dist = 200;
  }
  return dist;
}

//transforme les données d'un mouvement polaire definit par angle et dist, il utilise aussi les données de rotation du veicule et le paramétre d'angle de la roue pour calculer la vitesse que la roue doit avoir.
float Wheel_Speed(float angle,float dist,float rotation,float wheel_angle){
  float wheel_speed = dist*cos(angle-wheel_angle)+rotation;
  return wheel_speed;
}

void Set_motors_speed(float r1, float r2, float r3){
  stepperR1.setSpeed(r1);
  stepperR1.runSpeed();
  stepperR2.setSpeed(r2);
  stepperR2.runSpeed();
  stepperR3.setSpeed(r3);
  stepperR3.runSpeed();
}


//fonctions d'executions
 
//definision des donnée a execution unique 
void setup() {
  // ouverture du port serie pour la comunication et le debug
  Serial.begin(9600);

  //gestion des stepper
  pinMode (R1_ENABLE_PIN,OUTPUT);
  pinMode (R1_DIR_PIN,OUTPUT);
  pinMode (R1_STEP_PIN,OUTPUT);
  pinMode (R2_ENABLE_PIN,OUTPUT);
  pinMode (R2_DIR_PIN,OUTPUT);
  pinMode (R2_STEP_PIN,OUTPUT);
  pinMode (R3_ENABLE_PIN,OUTPUT);
  pinMode (R3_DIR_PIN,OUTPUT);
  pinMode (R3_STEP_PIN,OUTPUT);
  
  stepperR1.setEnablePin(R1_ENABLE_PIN);
  stepperR1.setPinsInverted(false, false, true); 
  stepperR1.setAcceleration(75.0);
  stepperR1.setMaxSpeed(1000000);

    
  stepperR2.setMaxSpeed(1000000);
  stepperR2.setAcceleration(75.0);
  stepperR2.setEnablePin(R2_ENABLE_PIN);
  stepperR2.setPinsInverted(false, false, true);      

  stepperR3.setMaxSpeed(1000000);
  stepperR3.setAcceleration(75.0);
  stepperR3.setEnablePin(R3_ENABLE_PIN);
  stepperR3.setPinsInverted(false, false, true);      
  
  stepperR1.enableOutputs();  
  stepperR2.enableOutputs();
  stepperR3.enableOutputs(); 
  digitalWrite (R1_ENABLE_PIN,LOW);
  digitalWrite (R2_ENABLE_PIN,LOW);
  digitalWrite (R3_ENABLE_PIN,LOW);
  armed = true;
}

//boucle principale
void loop() {
  //*
  if(Serial.available()>0){
    serial_income = Serial.read();
    if(serial_income=='x'){
      x = Serial.parseInt();
    }
    if(serial_income=='y'){
      y = Serial.parseInt();
    }
    if(serial_income=='z'){
       z = Serial.parseInt();
    }
    if(serial_income=='0'){
      x = 0;
      y = 0;
      z = 0;
    }
    if(serial_income =='d'){ // desarmer stepper 
      digitalWrite (R1_ENABLE_PIN,HIGH);
      digitalWrite (R2_ENABLE_PIN,HIGH);
      digitalWrite (R3_ENABLE_PIN,HIGH);
      armed = false;
      x = 0;
      y = 0;
      z = 0;
    }
    if(serial_income =='a'){ // armer les stepper
      digitalWrite (R1_ENABLE_PIN,LOW);
      digitalWrite (R2_ENABLE_PIN,LOW);
      digitalWrite (R3_ENABLE_PIN,LOW);
      armed = true;
    }
    
    //transformation des données 
    angle = XY_Move_To_Angle(x,y);
    dist = XY_Move_To_Dist(x,y);
    r1 = map(Wheel_Speed(angle,dist,z,(deg_to_rad*30)),-1,1,-1,1);
    r2 = map(Wheel_Speed(angle,dist,z,(deg_to_rad*150)),-1,1,-1,1);
    r3 = map(Wheel_Speed(angle,dist,z,(deg_to_rad*270)),-1,1,-1,1);
    
    //execution des données
    Serial.print("x = ");
    Serial.print(x);
    Serial.print(" y = ");
    Serial.print(y);
    Serial.print(" z = ");
    Serial.print(z);
    Serial.print(" armed = ");
    Serial.print(armed);
    Serial.print(" dist = ");
    Serial.print(dist);
    Serial.print(" angle = ");
    Serial.print(angle);
    Serial.print(" R1 = ");
    Serial.print(r1);
    Serial.print(" R2 = ");
    Serial.print(r2);
    Serial.print(" R3 = ");
    Serial.println(r3);
  }
  
  Set_motors_speed(r1,r2,r3);
}
