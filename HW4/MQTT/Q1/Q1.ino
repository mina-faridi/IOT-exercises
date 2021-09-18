#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


onstexpr uint8_t reset = D3;
constexpr uint8_t SS_PIN = D8;

const char* ssid = "Galaxy A20s6169";
const char* password = "secret2349";
const char* mqttIp = "192.168.1.102";

int LED_1 = 16;
int LED_2 = 5;

WiFiClient espClient;
PubSubClient client(espClient);

MFRC522 mfrc522(SS_PIN, reset);

void wifi_setup() {
  delay(100);
  Serial.print("client is connecting to ");
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
  Serial.println("IP address is: ");
  Serial.println(WiFi.localIP());
}

void call_back(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  if(payload[0] == 48) 
  {
    digitalWrite(LED1, HIGH); 
    digitalWrite(LED2, LOW);
    Serial.println("Access Denied!" ); 
    Serial.println("Yellow LED is ON!" ); 
  } 
  else if(payload[0] == 49) 
  {
    digitalWrite(LED2, HIGH); 
    digitalWrite(LED1, LOW); 
    Serial.println("Access Allowed!" );
    Serial.println("Green LED is ON!" );
  }
  Serial.println();
  Serial.println("-----------------------");
}

void reconnect() {
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Creating a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("responseMQTT");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in a few seconds");
      delay(20000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  wifi_setup();
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW); 
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW); 
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  client.setServer(mqttIp, 1883);
  client.setCallback(call_back);
}

void loop() {
  client.loop();
  if (!client.connected()) {
    reconnect();
  }
// Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) 
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
  Serial.print("UID tag:");
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
  delay(1000);

  char publish[50];
  content.substring(1).toCharArray(publish,50);
  client.publish("requestMQTT",publish);
  Serial.println(content.substring(1));
  delay(500);
}
