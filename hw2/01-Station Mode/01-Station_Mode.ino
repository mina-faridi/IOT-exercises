#include "ESP8266WiFi.h"


const char* ssid     = "DESKTOP-9FIKE1F 8110";         
const char* password = "abcdabcd";     
void setup() {
  Serial.begin(9600);         
  delay(10);
  
 }


void loop(){
  Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" network(s) found");
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
   

 WiFi.begin(ssid, password);           
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());        
  }
