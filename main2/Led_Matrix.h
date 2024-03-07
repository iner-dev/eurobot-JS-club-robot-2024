#ifndef MATRIX_LED
#define MATRIX_LED
#include "Arduino.h" 

class Led_Matrix{
  private : 
    int DIN_pin;
    int CS_pin;
    int CLK_pin;
    int intToHex(int);
    void write_pix(int);
    void write_line(int, int);
  public : 
    Led_Matrix(int,int,int);
    void init_matrix();
    void Write(int);
};

#endif
