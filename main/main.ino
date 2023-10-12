
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

float XY_Move_To_Dist(float x,float y){
  float dist = sqrt(sq(x)+sq(y));
  if(dist>100){
    dist = 100
  }
}

float x;
float y;
float z;
float angle;
float dist;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  x = map(analogRead(A0),0,1023,-50,50);
  y = map(analogRead(A2),0,1023,-50,50);
  angle = XY_Move_To_Angle(x,y);
  dist = XY_Move_To_Dist(x,y);





  
  Serial.print("x = ");
  Serial.print(x);
  Serial.print(" y = ");
  Serial.print(y);
  Serial.print(" dist = ");
  Serial.print(dist);
  Serial.print(" angle = ");
  Serial.println(angle);
  delay(500);
}
