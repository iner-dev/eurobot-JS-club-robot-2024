#include "animation.h"

Led_Matrix M1(53,51,49);
Led_Matrix M2(52,50,48);

animation anim(M1,M2);
void setup() {
  anim.start();
}

void loop() {
  anim.run();
}
