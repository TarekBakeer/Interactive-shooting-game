#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SevSeg.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
//pins 21 and 22 used for lcd
//pins
int button1 = 12;
int button2 = 13;
int buzzer = 15;
//sev seg
int a = 23;
int b = 18;
int c = 26;
int d = 25;
int e = 33;
int f = 19;
int g = 27;
int d1 = 4;
int d4 = 14;
//buttons
int lastbuttonState1 = HIGH;
int lastbuttonState2 = HIGH;
//menu
bool onePlayer = false;
bool twoPlayer = false;
bool easy = false;
bool med = false;
bool hard = false;
bool options = true; 
bool menuMusic = true;
//  counters 
int s = 0;
int i = 0;
int n = 0;
int m = 0;
//timing
unsigned long interval = 0;
unsigned long last = 0;
// communication
bool messageRE = false;
bool cUpdated = false;

//random generator
int test[9] = {2,3,1,3,2,1,3,2};
int sequence[9] ={};
void randomNumbers(int array[], int size, int min, int max)
{
  for (int r = 0; r < size; r++)
  {
    array[r] = random(min, max + 1);
    
    // Check if the current random number is equal to the previous number
    while (r > 0 && array[r] == array[r - 1])
    {
      array[r] = random(min, max + 1);
    }
  }
}

//MAC addresses
uint8_t receiver2Address[] = {0xB0, 0xA7, 0x32, 0x28, 0xD4, 0x64};
uint8_t receiver3Address[] = {0xE0, 0x5A, 0x1B, 0xCB, 0x37, 0x60};
uint8_t receiver4Address[] = {0xB0, 0xA7, 0x32, 0x16, 0x2E, 0x6C};
uint8_t receiver5Address[] = {0xB0, 0xA7, 0x32, 0x15, 0x44, 0xF4};



void setup() {

  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    delay(1000);
    ESP.restart();
  }
  esp_now_peer_info_t peerInfo2 = {};
  esp_now_peer_info_t peerInfo3 = {};
  esp_now_peer_info_t peerInfo4 = {};
  esp_now_peer_info_t peerInfo5 = {};
  memcpy(peerInfo2.peer_addr, receiver2Address, 6);
  memcpy(peerInfo3.peer_addr, receiver3Address, 6);
  memcpy(peerInfo4.peer_addr, receiver4Address, 6);
  memcpy(peerInfo5.peer_addr, receiver5Address, 6); 
  esp_now_add_peer(&peerInfo2);
  esp_now_add_peer(&peerInfo3);
  esp_now_add_peer(&peerInfo4);
  esp_now_add_peer(&peerInfo5);

  // lcd start up msg
  lcd.begin();
	lcd.backlight();
	lcd.print("Press to start");
  //pin modes
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(d1,OUTPUT);







  

// print random numbers 
  randomNumbers(sequence, 9, 1, 3);
  
  // register call back 
  esp_now_register_recv_cb(receiveCallback);
}

  

// receiving message
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen) {

  String message = String((char *)data);
  if (message == "newCycle")
  {

    cUpdated = false;
  }
  
if (!cUpdated)
{
  if (message == "sensor:Triggered")
  {
    digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
    i++;
    s++;
    messageRE = true;

    
      cUpdated = true;
  }
  else if (message == "timeFailed") 
  {
    i++;
    messageRE = true;
      cUpdated = true;

  }
}


}

void loop() {
 Serial.println(i);
 // Serial.println(cUpdated);
  unsigned long current = millis();
  int button1state = digitalRead(button1);
  // Turn on LED on ESP32 #2
  if (button1state == LOW && lastbuttonState1 == HIGH)
   { n ++;
   digitalWrite(buzzer,HIGH);
delay(200);
digitalWrite(buzzer,LOW);
     }
    lastbuttonState1 = button1state;
  //Serial.println(n);

      int button2state = digitalRead(button2);
  // Turn on LED on ESP32 #2
  if (button2state == LOW && lastbuttonState2 == HIGH && n>1)
   { m ++;
   digitalWrite(buzzer,HIGH);
delay(200);
digitalWrite(buzzer,LOW);
     }
    lastbuttonState2 = button2state;
      //Serial.println(m);


  if (m == 2 && menuMusic)
  {
  uint8_t menuMusicOn[] = "menuMusicOn";
  esp_now_send(receiver5Address, menuMusicOn, sizeof(menuMusicOn));
  menuMusic = false;
  }

   if (n == 1 )
{  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("One player");
  lcd.setCursor(0, 1);
  lcd.print("Two Players?");


}

if (n==2)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("One player");
}

if (n==3)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Two players");
}
if (n>3 && m == 0)
{
  n = 2;
}

if (n == 2 && m == 1)
{
  onePlayer = true;
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Easy Med or Hard");
}
/*if (n == 3 && m == 1)
{
  twoPlayer = true;
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Easy Med or Hard");
}*/
if (m>1 && n<1)
{
m =1;
}
if (n == 4)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Easy");
  
}
if (n == 5)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Med");
}
if (n == 6)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hard");
}
if (n>6 && m==1)
{
  n = 4;
}

if (n== 4 and m == 2)
{
  easy = true;
   interval = 6000;
}
if (n== 5 and m == 2)
{
  med = true;
    interval = 4000;

}
if (n== 6 and m == 2)
{
  hard = true;
     interval = 2000;

}

if (onePlayer && easy)
{
  if(options)
  {
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game starting...");
  delay(1050);
  lcd.clear();
    lcd.setCursor(7,0);
  lcd.print("3");
  delay(500);
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print("2");
  delay(500);
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print("1");
  delay(500);
  options = false;
  }
  if(!options)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SHOOT!");
    
  }
  Serial.println(s);
if (s == 0)
    { 
      zero();
      digit1();
    }

    else if(s == 1)
    {
      one();
      digit1();
    }
    else if(s == 2)
    {
      two();
      digit1();
    }
     else if(s == 3)
    {
      three();
      digit1();
    }
     else if(s == 4)
    {
      four();
      digit1();
    }
     else if(s == 5)
    {
      five();
      digit1();
    }
     else if(s == 6)
    {
      six();
      digit1();
    }
     else if(s == 7)
    {
      seven();
      digit1();
    }
     else if(s == 8)
    {
      eight();
      digit1();
    }
     else if(s == 9)
    {
      nine();
      digit1();
    }
  //Serial.print(interval);
  if (sequence[i] == 1 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonEasy" ;
   esp_now_send(receiver2Address, nodeOn, sizeof(nodeOn));

  }
   if (sequence[i] == 2 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonEasy" ;
   esp_now_send(receiver3Address, nodeOn, sizeof(nodeOn));

  }
  if (sequence[i] == 3 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonEasy" ;
   esp_now_send(receiver4Address, nodeOn, sizeof(nodeOn));

  }
  if (messageRE)
  {   

    
    uint8_t nodeoff[] = "nodeoff" ;
   esp_now_send(receiver2Address, nodeoff, sizeof(nodeoff));
   esp_now_send(receiver3Address, nodeoff, sizeof(nodeoff));
   esp_now_send(receiver4Address, nodeoff, sizeof(nodeoff));
   Serial.println("Node OFF message SENT");
    messageRE = false;
  }
  if(i == 9)
  {
     uint8_t menuMusicOff[] = "menuMusicOff";
  esp_now_send(receiver5Address, menuMusicOff, sizeof(menuMusicOff));
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game over");
    lcd.setCursor(0,1);
    lcd.print("Try again!" );
  }
}


if (onePlayer && med)
{
  if(options)
  {
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game starting...");
  delay(1050);
  lcd.clear();
    lcd.setCursor(7,0);
  lcd.print("3");
  delay(500);
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print("2");
  delay(500);
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print("1");
  delay(500);
  options = false;
  }
  if(!options)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SHOOT!");
    
  }
  Serial.println(s);
if (s == 0)
    { 
      zero();
      digit1();
    }

    else if(s == 1)
    {
      one();
      digit1();
    }
    else if(s == 2)
    {
      two();
      digit1();
    }
     else if(s == 3)
    {
      three();
      digit1();
    }
     else if(s == 4)
    {
      four();
      digit1();
    }
     else if(s == 5)
    {
      five();
      digit1();
    }
     else if(s == 6)
    {
      six();
      digit1();
    }
     else if(s == 7)
    {
      seven();
      digit1();
    }
     else if(s == 8)
    {
      eight();
      digit1();
    }
     else if(s == 9)
    {
      nine();
      digit1();
    }
  //Serial.print(interval);
  if (sequence[i] == 1 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonMed" ;
   esp_now_send(receiver2Address, nodeOn, sizeof(nodeOn));

  }
   if (sequence[i] == 2 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonMed" ;
   esp_now_send(receiver3Address, nodeOn, sizeof(nodeOn));

  }
  if (sequence[i] == 3 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonMed" ;
   esp_now_send(receiver4Address, nodeOn, sizeof(nodeOn));

  }
  if (messageRE)
  {   

    
    uint8_t nodeoff[] = "nodeoff" ;
   esp_now_send(receiver2Address, nodeoff, sizeof(nodeoff));
   esp_now_send(receiver3Address, nodeoff, sizeof(nodeoff));
   esp_now_send(receiver4Address, nodeoff, sizeof(nodeoff));
   Serial.println("Node OFF message SENT");
    messageRE = false;
  }
  if(i == 9)
  {
     uint8_t menuMusicOff[] = "menuMusicOff";
  esp_now_send(receiver5Address, menuMusicOff, sizeof(menuMusicOff));
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game over");
    lcd.setCursor(0,1);
    lcd.print("Try again!" );
  }
}
if (onePlayer && hard)
{
  if(options)
  {
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game starting...");
  delay(1050);
  lcd.clear();
    lcd.setCursor(7,0);
  lcd.print("3");
  delay(500);
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print("2");
  delay(500);
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print("1");
  delay(500);
  options = false;
  }
  if(!options)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SHOOT!");
    
  }
  Serial.println(s);
if (s == 0)
    { 
      zero();
      digit1();
    }

    else if(s == 1)
    {
      one();
      digit1();
    }
    else if(s == 2)
    {
      two();
      digit1();
    }
     else if(s == 3)
    {
      three();
      digit1();
    }
     else if(s == 4)
    {
      four();
      digit1();
    }
     else if(s == 5)
    {
      five();
      digit1();
    }
     else if(s == 6)
    {
      six();
      digit1();
    }
     else if(s == 7)
    {
      seven();
      digit1();
    }
     else if(s == 8)
    {
      eight();
      digit1();
    }
     else if(s == 9)
    {
      nine();
      digit1();
    }
  //Serial.print(interval);
  if (sequence[i] == 1 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonHard" ;
   esp_now_send(receiver2Address, nodeOn, sizeof(nodeOn));

  }
   if (sequence[i] == 2 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonHard" ;
   esp_now_send(receiver3Address, nodeOn, sizeof(nodeOn));

  }
  if (sequence[i] == 3 && !cUpdated)
  {
   uint8_t nodeOn[] = "nodeonHard" ;
   esp_now_send(receiver4Address, nodeOn, sizeof(nodeOn));

  }
  if (messageRE)
  {   

    
    uint8_t nodeoff[] = "nodeoff" ;
   esp_now_send(receiver2Address, nodeoff, sizeof(nodeoff));
   esp_now_send(receiver3Address, nodeoff, sizeof(nodeoff));
   esp_now_send(receiver4Address, nodeoff, sizeof(nodeoff));
   Serial.println("Node OFF message SENT");
    messageRE = false;
  }
  if(i == 9)
  {
     uint8_t menuMusicOff[] = "menuMusicOff";
  esp_now_send(receiver5Address, menuMusicOff, sizeof(menuMusicOff));
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game over");
    lcd.setCursor(0,1);
    lcd.print("Try again!" );
  }
}

}
void zero(){
digitalWrite(a, HIGH);
digitalWrite(b, HIGH);
digitalWrite(c, HIGH);
digitalWrite(d, HIGH);
digitalWrite(e, HIGH);
digitalWrite(f, HIGH);
digitalWrite(g, LOW);
  }
  void one(){
digitalWrite(a, LOW);
digitalWrite(b, HIGH);
digitalWrite(c, HIGH);
digitalWrite(d, LOW);
digitalWrite(e, LOW);
digitalWrite(f, LOW);
digitalWrite(g, LOW);
  }
   void two(){
digitalWrite(a, HIGH);
digitalWrite(b, HIGH);
digitalWrite(c, LOW);
digitalWrite(d, HIGH);
digitalWrite(e, HIGH);
digitalWrite(f, LOW);
digitalWrite(g, HIGH);
  }
  void three(){
digitalWrite(a, HIGH);
digitalWrite(b, HIGH);
digitalWrite(c, HIGH);
digitalWrite(d, HIGH);
digitalWrite(e, LOW);
digitalWrite(f, LOW);
digitalWrite(g, HIGH);
  }
  void four(){
digitalWrite(a, LOW);
digitalWrite(b, HIGH);
digitalWrite(c, HIGH);
digitalWrite(d, LOW);
digitalWrite(e, LOW);
digitalWrite(f, HIGH);
digitalWrite(g, HIGH);
  }
void five(){
digitalWrite(a, HIGH);
digitalWrite(b, LOW);
digitalWrite(c, HIGH);
digitalWrite(d, HIGH);
digitalWrite(e, LOW);
digitalWrite(f, HIGH);
digitalWrite(g, HIGH);
  }
  void six(){
digitalWrite(a, HIGH);
digitalWrite(b, LOW);
digitalWrite(c, HIGH);
digitalWrite(d, HIGH);
digitalWrite(e, HIGH);
digitalWrite(f, HIGH);
digitalWrite(g, HIGH);
  }
  void seven(){
digitalWrite(a, HIGH);
digitalWrite(b, HIGH);
digitalWrite(c, HIGH);
digitalWrite(d, LOW);
digitalWrite(e, LOW);
digitalWrite(f, LOW);
digitalWrite(g, LOW);
  }
  void eight(){
digitalWrite(a, HIGH);
digitalWrite(b, HIGH);
digitalWrite(c, HIGH);
digitalWrite(d, HIGH);
digitalWrite(e, HIGH);
digitalWrite(f, HIGH);
digitalWrite(g, HIGH);
  }
    void nine(){
digitalWrite(a, HIGH);
digitalWrite(b, HIGH);
digitalWrite(c, HIGH);
digitalWrite(d, LOW);
digitalWrite(e, LOW);
digitalWrite(f, HIGH);
digitalWrite(g, HIGH);
  }
  void digit1()
  {
    digitalWrite(d1,LOW);
    digitalWrite(d4,HIGH);
  }
  void digit4()
  {
    digitalWrite(d1,HIGH);
    digitalWrite(d4,LOW);
  }
  
  



