int led1 = 32;
int led2 = 27;
int echo = 18;
int trig = 5;
int c = 0;

 unsigned long interval = 0;
   unsigned long last = 0;
   unsigned long current = 0;

bool sensorTrig = false;
bool timeUpdated = false;
bool timeFail = false;
bool cUpdated = true;
bool nodeoff = false;
#include <esp_now.h>
#include <WiFi.h>

// Replace with the MAC address of ESP32 #1
uint8_t mainAddress[] = {0x24, 0xDC, 0xC3, 0x98, 0xFB, 0x50};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

 
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);



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
float readSensor()
{

  digitalWrite(5, LOW);
  delay(2);
  digitalWrite(5, HIGH);
  delay(10);
  digitalWrite(5, LOW);
  float distance = pulseIn(18, HIGH)/58;
  return distance;
}

void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen) 
{
  String message = String((char *)data);
   if (message == "nodeonEasy" && !timeUpdated)
    {
     last = current;
     timeUpdated = true;
    }

    if (message == "nodeonMed" && !timeUpdated)
    {
     last = current;
     timeUpdated = true;
    }
     if (message == "nodeonHard" && !timeUpdated)
    {
     last = current;
     timeUpdated = true;
    }



  if (message == "nodeonEasy" && !sensorTrig && !timeFail && cUpdated)
   {
     interval = 6000;
    float distance = readSensor();
   // Serial.println(current);
      Serial.println(last);

//Serial.println(distance);
digitalWrite(led1, HIGH); 
digitalWrite(led2, HIGH); 
    if(distance <35 && distance >0) 
    {
      sensorTrig = true;
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW); 
    cUpdated = false;
    }
    else if(current - last >= interval )
    {
      timeFail = true;
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW); 
        cUpdated = false;

    }
  }
   if (message == "nodeonMed" && !sensorTrig && !timeFail && cUpdated)
   {
     interval = 4000;
    float distance = readSensor();
   // Serial.println(current);
      Serial.println(last);

//Serial.println(distance);
digitalWrite(led1, HIGH); 
digitalWrite(led2, HIGH); 
    if(distance <35 && distance >0) 
    {
      sensorTrig = true;
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW); 
    cUpdated = false;
    }
    else if(current - last >= interval )
    {
      timeFail = true;
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW); 
        cUpdated = false;

    }
  }  
   if (message == "nodeonHard" && !sensorTrig && !timeFail && cUpdated)
   {
     interval = 2000;
    float distance = readSensor();
   // Serial.println(current);
      Serial.println(last);

//Serial.println(distance);
digitalWrite(led1, HIGH); 
digitalWrite(led2, HIGH); 
    if(distance <35 && distance >0) 
    {
      sensorTrig = true;
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW); 
    cUpdated = false;
    }
    else if(current - last >= interval )
    {
      timeFail = true;
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW); 
        cUpdated = false;

    }
  } 

  if(message == "nodeoff")
    { 
      Serial.println("NODE OFF RECEIVED");
      digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW); 
    timeUpdated = false;
    cUpdated = true;
    nodeoff = true;

    }
}

void loop()
 {

    current = millis();

 if (sensorTrig)
{ 
    uint8_t sensorTriggered[] = "sensor:Triggered";
  esp_now_send(mainAddress, sensorTriggered, sizeof(sensorTriggered));
  sensorTrig = false;
}
if (timeFail)
{

  uint8_t timeFailed[] = "timeFailed";
  esp_now_send(mainAddress, timeFailed, sizeof(timeFailed));
  timeFail = false;
}
if (nodeoff)
{
           Serial.println("NODE OFF RECEIVED");

  uint8_t newCycle[] = "newCycle";
  esp_now_send(mainAddress, newCycle, sizeof(newCycle));
  nodeoff = false;
}


}




