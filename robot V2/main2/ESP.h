#ifndef ESP
#define ESP
const char values_esp[] = {'S','c'};
int len = sizeof(values_esp)/sizeof(values_esp[0]);
bool founded;

void esp_update_serial_input(char index, int val){
  founded = false;
  for(int i = 0; i < len and !founded ; i++){
    if(values_esp[i] == index){
      founded = true;
      Serial2.println(values_esp[i]+String(val));
    }
  }
}


#endif
