#include <Wire.h>

void receiveData(int byteCount) {
  if(Wire.available()){
    for(int i;i<Wire.available();i++) Serial.print(char(Wire.read()));
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(1);
  Wire.onReceive(receiveData);
}

void loop() {
  if(Wire.available()){
    Serial.println(Wire.read());
  }
}
