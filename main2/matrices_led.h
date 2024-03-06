// definition des modeles des chifres des paneaux leds
int nothing[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
int arrow[8] = {B00011000,B00111100,B01111110,B11111111,B00011000,B00011000,B00011000,B00011000};
int chifres[10][8] = {
  {
    B00000000, //0
    B01111110,
    B10000001,
    B10000001,
    B10000001,
    B10000001,
    B01111110,
    B00000000
  },
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

void write_pix(int data,int DIN_pin,int CS_pin,int CLK_pin)
{
   digitalWrite(CS_pin, LOW);
  for (int i = 0; i < 8; i++)
  {
      digitalWrite(CLK_pin, LOW);
      digitalWrite(DIN_pin, data & 0x80); // masquage de donnée
      data = data << 1; // on décale les bits vers la gauche
      digitalWrite(CLK_pin, HIGH);
  }
}

void write_line(int adress, int data,int DIN_pin,int CS_pin,int CLK_pin)
{
  digitalWrite(CS_pin, LOW);
  write_pix(adress,DIN_pin,CS_pin,CLK_pin);
  write_pix(data,DIN_pin,CS_pin,CLK_pin);
  digitalWrite(CS_pin, HIGH);
}

void write_matrix(int *tab,int DIN_pin,int CS_pin,int CLK_pin)
{
  for (int i = 0; i < 8; i++){
    write_line(i + 1, tab[i],DIN_pin,CS_pin,CLK_pin);
  }
}

void init_matrix(int DIN_pin,int CS_pin,int CLK_pin)
{
  write_line(0x09, 0x00,DIN_pin,CS_pin,CLK_pin); //decoding BCD
  write_line(0X0A, 0x01,DIN_pin,CS_pin,CLK_pin); //brightness
  write_line(0X0B, 0x07,DIN_pin,CS_pin,CLK_pin); //scanlimit 8leds
  write_line(0X0C, 0x01,DIN_pin,CS_pin,CLK_pin); //power-down mode 0, normalmode1;
  write_line(0X0F, 0x00,DIN_pin,CS_pin,CLK_pin);
}

int intToHex(int x)
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
