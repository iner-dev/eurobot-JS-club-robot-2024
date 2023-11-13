#include <math.h>
#include <AccelStepper.h>


#define VD_STEP_PIN         A0  //54  // ex x
#define VD_DIR_PIN          A1  //55 //
#define VD_ENABLE_PIN       38  


#define VG_STEP_PIN         A6  //60  //A6   // ex y
#define VG_DIR_PIN          A7   //  61  //A7
#define VG_ENABLE_PIN       56


#define RD_STEP_PIN         46          // ex Z
#define RD_DIR_PIN          48
#define RD_ENABLE_PIN       A8  // 62  //A8

#define RG_STEP_PIN         26          // ex E0
#define RG_DIR_PIN          28
#define RG_ENABLE_PIN       24
 
AccelStepper stepperVD(AccelStepper::DRIVER, VD_STEP_PIN, VD_DIR_PIN);
AccelStepper stepperVG(AccelStepper::DRIVER, VG_STEP_PIN, VG_DIR_PIN);
AccelStepper stepperRD(AccelStepper::DRIVER, RD_STEP_PIN, RD_DIR_PIN);
AccelStepper stepperRG(AccelStepper::DRIVER, RG_STEP_PIN, RG_DIR_PIN);

unsigned long instantT;
int vitesse;
float angle;
int accel=100;
long duree;

void setup() {

    Serial.begin (9600);
    pinMode (VD_ENABLE_PIN,OUTPUT);
    pinMode (VD_DIR_PIN,OUTPUT);
    pinMode (VD_STEP_PIN,OUTPUT);
    pinMode (VG_ENABLE_PIN,OUTPUT);
    pinMode (VG_DIR_PIN,OUTPUT);
    pinMode (VG_STEP_PIN,OUTPUT);
    pinMode (RD_ENABLE_PIN,OUTPUT);
    pinMode (RD_DIR_PIN,OUTPUT);
    pinMode (RD_STEP_PIN,OUTPUT);
    pinMode (RG_ENABLE_PIN,OUTPUT);
    pinMode (RG_DIR_PIN,OUTPUT);
    pinMode (RG_STEP_PIN,OUTPUT);
    
    stepperVD.setEnablePin(VD_ENABLE_PIN);
    stepperVD.setPinsInverted(false, false, true); 
    stepperVD.setAcceleration(500.0);
    stepperVD.setMaxSpeed(3000.0);

    
    stepperVG.setMaxSpeed(3000.0);
    stepperVG.setAcceleration(500.0);
    stepperVG.setEnablePin(VG_ENABLE_PIN);
    stepperVG.setPinsInverted(false, false, true);      

    stepperRD.setMaxSpeed(3000.0);
    stepperRD.setAcceleration(500.0);
    stepperRD.setEnablePin(RD_ENABLE_PIN);
    stepperRD.setPinsInverted(false, false, true);      
    
    stepperRG.setMaxSpeed(3000.0);
    stepperRG.setAcceleration(500.0);
    stepperRG.setEnablePin(RG_ENABLE_PIN);
    stepperRG.setPinsInverted(false, false, true);   

    
  stepperRD.enableOutputs();
  stepperRG.enableOutputs();
  stepperVD.enableOutputs();  
  stepperVG.enableOutputs(); 
  digitalWrite (RG_ENABLE_PIN,LOW);
  digitalWrite (RD_ENABLE_PIN,LOW);
  digitalWrite (VD_ENABLE_PIN,LOW);
  digitalWrite (VG_ENABLE_PIN,LOW);

}

void loop() {
  



        avance (3000,2000);
/*
  
   stepperVD.stop();
  stepperVG.stop();
  stepperRG.stop ();
  stepperRD.stop ();
  
  digitalWrite (VD_ENABLE_PIN,HIGH);
  digitalWrite (VG_ENABLE_PIN,HIGH);
  digitalWrite (RD_ENABLE_PIN,HIGH);
  digitalWrite (RG_ENABLE_PIN,HIGH);
while (1);    
*/



  



}



void avance (int vit,int temps)                      // 0<=vit<10000  0<=temps
  {
 
  stepperRD.setSpeed(vit);
  stepperRG.setSpeed(vit);
  stepperVD.setSpeed(vit); 
  stepperVG.setSpeed(vit); 
  instantT=millis();
  while (millis()<(instantT+temps))
  {
  stepperRD.runSpeed();
  stepperRG.runSpeed();
  stepperVD.runSpeed();
  stepperVG.runSpeed();
  }
  }
