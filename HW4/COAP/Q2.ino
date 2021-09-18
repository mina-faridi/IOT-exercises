#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include "coap_client.h"

int LED_1 = 16;
int LED_2 = 5;

constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D8;

const char* ssid = "Galaxy A20s6169"
const char* password = "secret2348";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

IPAddress ip(192, 168, 1, 102);

coapClient coap;

int port =5683;

void callback_response(coapPacket &packet, IPAddress ip, int port) {
    Serial.println("Reponse:   ");
    char p[packet.payloadlen + 1];
    memcpy(p, packet.payload, packet.payloadlen);
    p[packet.payloadlen] = NULL;
    Serial.println(p);
    
    if (strcmp(p, "1") == 0){
      
      digitalWrite(D1, HIGH); 
      digitalWrite(D2, LOW); 
      Serial.println("Access Allowed!" );
      Serial.println("Green LED is ON!" );
    } 
    else {
      digitalWrite(D2, HIGH); 
      digitalWrite(D1, LOW);
      Serial.println("Access Denied!" ); 
      Serial.println("Yellow LED is ON!" ); 
   }
}

 
void setup() {
Serial.begin(115200);
pinMode(LED_1, OUTPUT);
digitalWrite(LED_1, LOW); 
pinMode(LED_2, OUTPUT);
digitalWrite(LED_2, LOW);
delay(1000);
  
Serial.println();
Serial.println("Connecting to WiFi");
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(100);
Serial.print(".");
}
 
Serial.println();
Serial.print("WiFi connected with ip ");
Serial.println(WiFi.localIP());
SPI.begin();      // Initiate  SPI bus
mfrc522.PCD_Init();   // Initiate MFRC522 
coap.response(callback_response);

coap.start();
 
}
 
void loop() {
 // Looking for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
      
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  delay(3000);
  char b[50];
  content.substring(1).toCharArray(b,50);
  coap.put(ip, port, "path", b, strlen(b));
  Serial.println(content.substring(1));
  delay(1000);
  coap.loop();
}
