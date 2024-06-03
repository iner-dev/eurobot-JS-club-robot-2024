#include "Led_Matrix.h" 
Led_Matrix::Led_Matrix(int DIN,int CS,int CLK){
  DIN_pin = DIN;
  pinMode(DIN_pin, OUTPUT);
  CS_pin = CS;
  pinMode(CS_pin, OUTPUT);
  CLK_pin = CLK;
  pinMode(CLK_pin, OUTPUT);
  init_matrix();
}

int Led_Matrix::intToHex(int x)
{
  switch (x)
  {
      case 0: return 0x01; break; //LED sur la première case
      case 1: return 0x02; break; //LED sur 2 case
      case 2: return 0x04; break; //LED sur 3 case
      case 3: return 0x08; break; //LED sur 4 case
      case 4: return 0x10; break; //LED sur 5 case
      case 5: return 0x20; break; //LED sur 6 case
      case 6: return 0x40; break; //LED sur 7 case
      case 7: return 0x80; break; //LED sur 8 case
  }
}

void Led_Matrix::write_pix(int data)
{
  digitalWrite(CS_pin, LOW);
  for (int i = 0; i < 8; i++)
  {
      digitalWrite(CLK_pin, LOW);
      digitalWrite(DIN_pin, data & 0x80); // masquage de donnée
      data = data << 1; // on décale les bits vers la droite
      digitalWrite(CLK_pin, HIGH);
  }
}

void Led_Matrix::i_write_pix(int data)
{
  digitalWrite(CS_pin, LOW);
  data = data << 7; // on décale les bits vers la droite
  for (int i = 0; i < 8; i++)
  {
      digitalWrite(CLK_pin, LOW);
      digitalWrite(DIN_pin, data & 0x80); // masquage de donnée
      data = data >> 1; // on décale les bits vers la droite
      digitalWrite(CLK_pin, HIGH);
  }
}

void Led_Matrix::write_line(int adress, int data)
{
  digitalWrite(CS_pin, LOW);
  write_pix(adress);
  write_pix(data);
  digitalWrite(CS_pin, HIGH);
}

void Led_Matrix::i_write_line(int adress, int data)
{
  digitalWrite(CS_pin, LOW);
  write_pix(adress);
  i_write_pix(data);
  digitalWrite(CS_pin, HIGH);
}

void Led_Matrix::init_matrix()
{
  write_line(0x09, 0x00); //decoding BCD
  write_line(0X0A, 0x01); //brightness
  write_line(0X0B, 0x07); //scanlimit 8leds
  write_line(0X0C, 0x01); //power-down mode 0, normalmode1;
  write_line(0X0F, 0x00);
}

void Led_Matrix::Write(int tab[8])
{
  for (int i = 0; i < 8; i++) {
    write_line(i + 1, tab[i]);
  }
}

void Led_Matrix::i_Write(int tab[8])
{
  for (int i = 0; i < 8; i++) {
    i_write_line(i + 1, tab[7-i]);
  }
}

const int Led_Matrix::start_animation_1[18][8] = {
  {
    B00000000,
    B00011000,
    B00111100,
    B00111100,
    B00011000,
    B00000000,
    B00000000,
    B00000000
  },{
    B00011000,
    B00111100,
    B01100110,
    B01100110,
    B00111100,
    B00011000,
    B00000000,
    B00000000
  },{
    B00111100,
    B01100110,
    B11000011,
    B11000011,
    B01100110,
    B00111100,
    B00011000,
    B00000000
  },{
    B11000011,
    B10000001,
    B10011001,
    B10000001,
    B11000011,
    B01111110,
    B00111100,
    B00000000
  },{
    B10000001,
    B00011000,
    B00111100,
    B00011000,
    B10000001,
    B11000011,
    B01111110,
    B00111100
  },{
    B00011000,
    B00111100,
    B01100110,
    B00111100,
    B00011000,
    B10000001,
    B11000011,
    B01111110
  },{
    B00111100,
    B01100110,
    B11000011,
    B01100110,
    B00111100,
    B00011000,
    B10000001,
    B11000011
  },{
    B01100110,
    B11000011,
    B10000001,
    B11000011,
    B01100110,
    B00111100,
    B00011000,
    B10000001
  },{
    B11000011,
    B10000001,
    B00011000,
    B10000001,
    B11000011,
    B01100110,
    B00111100,
    B00011000
  },{
    B10000001,
    B00011000,
    B00011000,
    B00011000,
    B10000001,
    B11000011,
    B01100110,
    B00111100
  },{
    B00011000,
    B00111100,
    B00111100,
    B00111100,
    B00011000,
    B10000001,
    B11000011,
    B01100110
  },{
    B00000000,
    B00011000,
    B00111100,
    B01111110,
    B00111100,
    B00011000,
    B10000001,
    B11000011
  },{
    B10000001,
    B00111100,
    B01111110,
    B01111110,
    B01111110,
    B00111100,
    B00000000,
    B10000001
  },{
    B10000001,
    B00011000,
    B00111100,
    B00111100,
    B00111100,
    B00011000,
    B00000000,
    B00000000
  },{
    B10000001,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00000000,
    B00000000
  },{
    B10000001,
    B00011000,
    B00111100,
    B00111100,
    B00111100,
    B00011000,
    B00000000,
    B00000000
  },{
    B10000001,
    B00111100,
    B01111110,
    B01111110,
    B01111110,
    B00111100,
    B00000000,
    B00000000
  },{
    B10000001,
    B00111100,
    B01111110,
    B01111110,
    B01111110,
    B00111100,
    B00000000,
    B00000000
  }
};
const int Led_Matrix::start_animation_2[18][8] = {
  {
    B00000000,
    B00000000,
    B00000000,
    B00011000,
    B00111100,
    B00111100,
    B00011000,
    B00000000
  },{
    B00000000,
    B00000000,
    B00011000,
    B00111100,
    B01100110,
    B01100110,
    B00111100,
    B00011000
  },{
    B00000000,
    B00011000,
    B00111100,
    B01100110,
    B11000011,
    B11000011,
    B01100110,
    B00111100
  },{
    B00000000,
    B00111100,
    B01111110,
    B11000011,
    B10000001,
    B10011001,
    B10000001,
    B11000011
  },{
    B00111100,
    B01111110,
    B11000011,
    B10000001,
    B00011000,
    B00111100,
    B00011000,
    B10000001
  },{
    B01111110,
    B11000011,
    B10000001,
    B00011000,
    B00111100,
    B01100110,
    B00111100,
    B00011000
  },{
    B11000011,
    B10000001,
    B00011000,
    B00111100,
    B01100110,
    B11000011,
    B01100110,
    B00111100
  },{
    B10000001,
    B00011000,
    B00111100,
    B01100110,
    B11000011,
    B10000001,
    B11000011,
    B01100110
  },{
    B00011000,
    B00111100,
    B01100110,
    B11000011,
    B10000001,
    B00011000,
    B10000001,
    B11000011
  },{
    B00111100,
    B01100110,
    B11000011,
    B10000001,
    B00011000,
    B00011000,
    B00011000,
    B10000001
  },{
    B01100110,
    B11000011,
    B10000001,
    B00011000,
    B00111100,
    B00111100,
    B00111100,
    B00011000
  },{
    B11000011,
    B10000001,
    B00011000,
    B00111100,
    B01111110,
    B00111100,
    B00011000,
    B00000000
  },{
    B10000001,
    B00000000,
    B00111100,
    B01111110,
    B01111110,
    B01111110,
    B00111100,
    B10000001
  },{
    B00000000,
    B00000000,
    B00011000,
    B00111100,
    B00111100,
    B00111100,
    B00011000,
    B10000001
  },{
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B10000001
  },{
    B00000000,
    B00000000,
    B00011000,
    B00111100,
    B00111100,
    B00111100,
    B00011000,
    B10000001
  },{
    B00000000,
    B00000000,
    B00111100,
    B01111110,
    B01111110,
    B01111110,
    B00111100,
    B10000001
  },{
    B00000000,
    B00000000,
    B00111100,
    B01111110,
    B01111110,
    B01111110,
    B00111100,
    B10000001
  }
};
