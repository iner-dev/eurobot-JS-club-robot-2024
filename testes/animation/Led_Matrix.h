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
    void i_write_pix(int);
    void i_write_line(int, int);
    void init_matrix();
  public : 
    Led_Matrix(int,int,int);
    void Write(int[8]);
    void i_Write(int[8]);
    static const int start_animation_1[18][8];
    static const int start_animation_2[18][8];
    int nothing[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
    int chiffres[10][8] = {
    { B00000000, B01111110, B10000001, B10000001, B10000001, B10000001, B01111110, B00000000 }, //0
    {
      B00000000,  //1
      B00000000,
      B10000000,
      B10000001,
      B11111111,
      B10000000,
      B00000000,
      B00000000
    },
    {
      B00000000,  //2
      B01110000,
      B10001001,
      B10001001,
      B10001001,
      B10001001,
      B00000110,
      B00000000
    },
    {
      B00000000,  //3
      B10000001,
      B10001001,
      B10001001,
      B10001001,
      B10001001,
      B01110110,
      B00000000
    },
    {
      B00000000,  //4
      B00001111,
      B00010000,
      B00010000,
      B00010000,
      B00010000,
      B11111111,
      B00000000
    },
    {
      B00000000,  //5
      B00000110,
      B10001001,
      B10001001,
      B10001001,
      B10001001,
      B01110000,
      B00000000
    },
    {
      B00000000,  //6
      B01110110,
      B10001001,
      B10001001,
      B10001001,
      B10001001,
      B01110000,
      B00000000
    },
    {
      B00000000,  //7
      B00000001,
      B00000001,
      B00000001,
      B00000001,
      B00000001,
      B11111110,
      B00000000
    },
    {
      B00000000,  //8
      B01110110,
      B10001001,
      B10001001,
      B10001001,
      B10001001,
      B01110110,
      B00000000
    },
    {
      B00000000,  //9
      B00000110,
      B10001001,
      B10001001,
      B10001001,
      B10001001,
      B01110110,
      B00000000
    }
  };
};


#endif
