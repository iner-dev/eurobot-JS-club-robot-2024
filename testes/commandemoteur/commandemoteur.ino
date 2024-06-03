#include <Wire.h>

int val = 0;
float kp = 2.;
float ki = 1.;

#define ADDR_MOTEUR 0x01
void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  if (Serial.available())
  {
    String str = Serial.readStringUntil('\n');
    if (str[0]=='p')
    {
      kp = str.substring(1).toFloat();
      val = 0;
      Serial.print("kp : ");
      Serial.println(kp);

      for (int i=0; i<3; i++)
      {
        Wire.beginTransmission(ADDR_MOTEUR+i); // transmit to device #8
        Wire.write(val >> 8);      
        Wire.write(val & 0xFF);
        Wire.write(((uint8_t *)&kp)[3]);      
        Wire.write(((uint8_t *)&kp)[2]);      
        Wire.write(((uint8_t *)&kp)[1]);      
        Wire.write(((uint8_t *)&kp)[0]);      
        Wire.write(((uint8_t *)&ki)[3]);      
        Wire.write(((uint8_t *)&ki)[2]);      
        Wire.write(((uint8_t *)&ki)[1]);      
        Wire.write(((uint8_t *)&ki)[0]);   
        Wire.write(0);    
        Wire.endTransmission();
      }
    }
    else if (str[0]=='i')
    {
      ki = str.substring(1).toFloat();
      val = 0;

      Serial.print("ki : ");
      Serial.println(ki);

      for (int i=0; i<3; i++)
      {
        Wire.beginTransmission(ADDR_MOTEUR+i); // transmit to device #8
        Wire.write(val >> 8);      
        Wire.write(val & 0xFF);
        Wire.write(((uint8_t *)&kp)[3]);      
        Wire.write(((uint8_t *)&kp)[2]);      
        Wire.write(((uint8_t *)&kp)[1]);      
        Wire.write(((uint8_t *)&kp)[0]);      
        Wire.write(((uint8_t *)&ki)[3]);      
        Wire.write(((uint8_t *)&ki)[2]);      
        Wire.write(((uint8_t *)&ki)[1]);      
        Wire.write(((uint8_t *)&ki)[0]);      
        Wire.write(0);    
        Wire.endTransmission();
      }
    }
    else if (str[0]=='a')
    {
      val = str.substring(1).toInt();

      Wire.beginTransmission(ADDR_MOTEUR); // transmit to device #8
      Wire.write(val >> 8);      
      Wire.write(val & 0xFF); 
      Wire.write(0);    
      Wire.endTransmission();
    }
    else if (str[0]=='b')
    {
      val = str.substring(1).toInt();

      Wire.beginTransmission(ADDR_MOTEUR+1); // transmit to device #8
      Wire.write(val >> 8);      
      Wire.write(val & 0xFF); 
      Wire.write(0);    
      Wire.endTransmission();
    }
    else if (str[0]=='c')
    {
      val = str.substring(1).toInt();

      Wire.beginTransmission(ADDR_MOTEUR+2); // transmit to device #8
      Wire.write(val >> 8);      
      Wire.write(val & 0xFF); 
      Wire.write(0);    
      Wire.endTransmission();
    }
    else if (str[0]=='x')
    {
      val = str.substring(1).toInt();

      for (int i=0; i<3; i++)
      {
        Wire.beginTransmission(ADDR_MOTEUR+i); // transmit to device #8
        Wire.write(val >> 8);      
        Wire.write(val & 0xFF); 
        Wire.write(0);    
        Wire.endTransmission();
      }
    }
  }

  delay(10);

}
