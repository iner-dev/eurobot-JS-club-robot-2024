
//parametres
#include <AccelStepper.h>

//definition des  stepper et leurs pins

#define R1_STEP_PIN         2    // ex x
#define R1_DIR_PIN          3 
#define R1_ENABLE_PIN       8 

AccelStepper stepperR1(AccelStepper::DRIVER, R1_STEP_PIN, R1_DIR_PIN);

//fonctions d'executions
 
//definision des donnée a execution unique 
void setup() {
  // ouverture du port serie pour la comunication et le debug
  Serial.begin(9600);

  //gestion des stepper
  pinMode (R1_ENABLE_PIN,OUTPUT);
  pinMode (R1_DIR_PIN,OUTPUT);
  pinMode (R1_STEP_PIN,OUTPUT);
  
  stepperR1.setEnablePin(R1_ENABLE_PIN);
  stepperR1.setPinsInverted(false, false, true); 
  stepperR1.setAcceleration(75.0);
  stepperR1.setMaxSpeed(1000000);
  stepperR1.setSpeed(500);
}

//boucle principale
void loop() {
  //* 
  stepperR1.runSpeed();
}
