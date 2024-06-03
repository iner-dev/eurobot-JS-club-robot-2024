#include <ESP8266WiFi.h>
#include <espnow.h>

int id = 2;
uint8_t self_MAC[6];

// REPLACE WITH RECEIVER MAC Address
const int all_Adress_number = 4;
uint8_t all_broadcastAddress[4][6] = {
  {0x30, 0x83, 0x98, 0x80, 0x36, 0x9D},
  {0x84, 0xCC, 0xA8, 0xA0, 0x4A, 0xAD},
  {0x40, 0xF5, 0x20, 0x3D, 0x32, 0x7E},
  {0x30, 0x83, 0x98, 0x82, 0x25, 0x2D}
};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char index;
  int emiter_id;
  int value;
} struct_message;

// Create a struct_message called myData
struct_message myData;




// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(char(myData.index) + String(myData.value));
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Register peer
  for(int i = 0; i < all_Adress_number; i++){
    if(i + 1 != id){
      esp_now_add_peer(all_broadcastAddress[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    }
  }
}
 
void loop() {
  if (Serial.available()) {
    // Set values to send
    myData.index = Serial.read();
    myData.emiter_id = id;
    myData.value = Serial.parseInt();
    // Send message via ESP-NOW
    for(int i = 0; i < all_Adress_number; i++){
      if(i + 1 != id){
        esp_now_send(all_broadcastAddress[i], (uint8_t *) &myData, sizeof(myData));
      }
    }
  }
}
