#include <Wire.h>

int error;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

int SMBus_Send(int target, int order, int value){
  Wire.beginTransmission(target);
  Wire.write(order);
  Wire.write(value);
  return Wire.endTransmission();
}

void loop() {
  if(Serial.available()){
    SMBus_Send(1,Serial.read(),Serial.parseInt());
  }
}
