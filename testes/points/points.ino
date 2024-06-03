#include "Points.h"

Led_Matrix M1(53,51,49);
Led_Matrix M2(52,50,48);

Codeur C1(2, 32, false,false);

Points points(C1, M1, M2);
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  points.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  points.run();
}
