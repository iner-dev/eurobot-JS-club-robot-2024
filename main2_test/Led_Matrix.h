#ifndef LED_MATRIX 
#define LED_MATRIX
#include "Arduino.h" 

class Led_Matrix{
  private : 
    int DIN_pin;
    int CS_pin;
    int CLK_pin;
    int intToHex(int);
    void write_pix(int);
    void write_line(int, int);
    void init_matrix();
  public : 
    Led_Matrix(int,int,int);
    void Write(int[8]);
};


#endif
