//parametres
int presision = 20;

//variables
int pin_x = 0;
int pin_y = 2;
int pin_z = 4;
int last_x = 0;
int last_y = 0;
int last_z = 0;
int x = 0;
int y = 0;
int z = 0;

void setup() {
  // put your setup code here, to run once:
  delay(200);
  Serial.begin(9600);
  x = map(map(analogRead(pin_x),0,1023,-presision,presision),-presision,presision,-1000,1000);
  y = map(map(analogRead(pin_y),0,1023,-presision,presision),-presision,presision,-1000,1000);
  z = map(map(analogRead(pin_z),0,1023,-presision,presision),-presision,presision,-1000,1000);
  last_x = x;
  Serial.println("x"+String(x));
  last_y = y;
  Serial.println("y"+String(y));
  last_z = z;
  Serial.println("z"+String(z));
}

void loop() {
  // put your main code here, to run repeatedly:
  x = map(map(analogRead(pin_x),0,1023,-presision,presision),-presision,presision,-1000,1000);
  if(last_x != x){
    last_x = x;
    Serial.println("x"+String(x));
  }
  y = map(map(analogRead(pin_y),0,1023,-presision,presision),-presision,presision,-1000,1000);
  if(last_y != y){
    last_y = y;
    Serial.println("y"+String(y));
  }
  x = map(map(analogRead(pin_z),0,1023,-presision,presision),-presision,presision,-1000,1000);
  if(last_z != z){
    last_z = z;
    Serial.println("y"+String(y));
  }
  delay(10);
}
