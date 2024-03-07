
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

void write_matrix(int tab,int DIN_pin,int CS_pin,int CLK_pin)
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
