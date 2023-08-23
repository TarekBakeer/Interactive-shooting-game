#include <esp_now.h>
#include <WiFi.h>


int melody[] = {220, 220, 261, 330, 220, 220, 261, 330, 294, 294, 350, 440, 294, 294, 350, 440, 196, 196, 261, 294, 196, 196, 261, 294, 174, 174, 230, 261, 220, 220, 261, 330, 220};
int melody2[] = {110, 110, 110, 110, 110, 110, 110, 110, 146, 146, 146, 146, 146, 146, 146, 146, 98, 98, 98, 98, 98, 98, 98, 98, 98, 87, 87, 87, 87, 87, 87, 87, 87};


int buzzer = 13;



bool menuMusic = false;


uint8_t mainAddress[] = {0x24, 0xDC, 0xC3, 0x98, 0xFB, 0x50};


void setup() {

   Serial.begin(115200);
   pinMode(buzzer, OUTPUT);


  WiFi.mode(WIFI_STA);
 if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    delay(1000);
    ESP.restart();
 }
 esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mainAddress, 6);
esp_now_add_peer(&peerInfo);

  esp_now_register_recv_cb(receiveCallback);
}
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen) 
{
  String message = String((char *)data);
if (message == "menuMusicOn" )
{
  menuMusic = true;
}
if (message== "menuMusicOff")
{
  menuMusic = false;
}
}
  



void loop() {

if (menuMusic)
{

for (int thisNote = 0; thisNote < 32; thisNote++) {
    int noteDuration = 120;
    tone(buzzer, melody[thisNote], noteDuration);
    delay(noteDuration);
    noTone(buzzer);
    tone(buzzer, melody2[thisNote], noteDuration);
    delay(noteDuration);
    noTone(buzzer);

}

}
}
