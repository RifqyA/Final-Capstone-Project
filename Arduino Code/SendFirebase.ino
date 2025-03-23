#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
// Wifi network station credentials
#define WIFI_SSID "Rifqy's Galaxy A71"
#define WIFI_PASSWORD "rahasiaa"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "7130669244:AAHhU3kPaGEonEbwqyDzTupeb2Hllly636s"

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "186660074"

#define SS_PIN D2  // sda pin D2
#define RST_PIN D1 // RST (flash) pin D2


#define relay D4 //Relay Pin D4
#define BUZZER D3 //buzzer pin D8
#define LED_R D0 //LED R (RGB)
#define LED_G 3  //LED G (RGB) (pin RX0)
#define LED_B D8  //LED B (RGB)

MFRC522 mfrc522(SS_PIN, RST_PIN);        

byte i,j,f_cek;
String ID_TAG;
String ID_Card[10];
int jmlkartu;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

//============
void Red_ON(){
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);
}
//============
void Green_ON(){
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);
  digitalWrite(LED_B,LOW);
}
//============
void Blue_ON(){
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,HIGH);
}
//============
void readRFID(byte *buffer, byte bufferSize)
{
  ID_TAG="";
  for(byte i = 0;i<bufferSize; i++)
  {
    ID_TAG = ID_TAG+String(buffer[i], HEX);
  }
}
//=======================
void iot_rfid()
{
  int k = 1;
  int jmlkartu = 3;  
 Blue_ON(); 
 if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()){
    return;
  } 

  readRFID(mfrc522.uid.uidByte, mfrc522.uid.size);
  f_cek=0; 
  

  while(k < jmlkartu+1)
  {

    if(ID_TAG==ID_Card[k]){  
      f_cek=1;    
      Serial.print("Akses diberikan untuk Kartu :");
      Serial.println(k);    
      Green_ON();
      digitalWrite(relay,LOW);   
      bot.sendMessage(CHAT_ID, "Pintu Terbuka", ""); 
      delay(2000);
      digitalWrite(relay,HIGH);
    }
    k++;
  }

  if(f_cek==0){    
    Serial.println("Kartu Tidak Terdaftar !"); 

    Serial.println("Akses ditolak !");

    bot.sendMessage(CHAT_ID, "Kartu tidak terdaftar mencoba masuk !", "");
    Red_ON();
    digitalWrite(BUZZER, HIGH);
    delay(2000);
    digitalWrite(BUZZER, LOW);    
  }
     
}
//============================
void setup()
{
  ID_Card[1]="33b65dfe";
  ID_Card[2]="f3ecccfe";
  ID_Card[3]="9b2b889";
  
   
  Serial.begin(115200);
  delay(200);

  pinMode(relay, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(relay,HIGH); 

  Red_ON();  
  delay(100);   
  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
    
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}
//=======================
void loop() { 
  iot_rfid();
  delay(1000);
}
