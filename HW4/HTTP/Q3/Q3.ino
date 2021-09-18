#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>


int LED_PIN1 = 16;
int LED_PIN2 = 5;
 
constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D8;

const char* ssid = "Galaxy A20s6169";
const char* password = "secret2348";
String serverName = "http://192.168.1.102:8090/";

int requestNumber = 0;
int waitingForResponse = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);
 
void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
  digitalWrite(LED_PIN1, LOW); 
  pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN2, LOW); 
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi Network");
}

void getCommand() {
  String serverPath = serverName + "command";
      HTTPClient http;
      http.begin(serverPath);    //Specify request destination
      int httpCode = http.GET(); //Send the request
      if (httpCode > 0) {
        String payload = http.getString();
        if (payload.substring(1) == (String)requestNumber) {
          waitingForResponse = 0;
          Serial.println(payload.charAt(0));
          if(payload.charAt(0) == 48) {
            digitalWrite(LED_PIN1, HIGH); 
            digitalWrite(LED_PIN2, LOW);
            Serial.println("Access Denied!" ); 
            Serial.println("Yellow LED is ON!" ); 
          } 
          else if(payload.charAt(0) == 49) {
            digitalWrite(LED_PIN2, HIGH); 
            digitalWrite(LED_PIN1, LOW); 
            Serial.println("Access Allowed!" );
            Serial.println("Green LED is ON!" );
          }
          Serial.println();
          Serial.println("-----------------------");
        }
        else {
          delay(1000);
        }
      }
      else {
        Serial.println("An error ocurred while trying to get command");
        Serial.println(httpCode);
      }
      http.end();   //Close connection
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (waitingForResponse == 1) {
      getCommand();
    }
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
    requestNumber = requestNumber + 1;
    //Show UID on serial monitor
    Serial.println();
    Serial.print("UID tag:");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) {  
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    Serial.println();
    String serverPath = serverName + "submitValue" + "?value=" + content + requestNumber;
    HTTPClient http;
    http.begin(serverPath);    //Specify request destination
    int httpCode = http.GET(); //Send the request
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      Serial.println("value is successfully passed to the server. waiting for response...");
      waitingForResponse = 1;
    }
    else {
      Serial.println("An error ocurred while trying to submit data");
    }
    http.end();   //Close connection
  }
  delay(1000);
}
