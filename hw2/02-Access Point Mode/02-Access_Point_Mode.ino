#include <ESP8266WiFi.h>    

IPAddress local_IP(192,168,4,3);
IPAddress gateway(192,168,4,3);
IPAddress subnet(255,255,255,0);

const char* ssid     = "NodeMCU";    
const char* password = "12345678";   

void setup() {
 Serial.begin(9600);
  delay(7000);
  WiFi.mode(WIFI_AP);
  Serial.println('\n');
  WiFi.softAP(ssid, password);           
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());        
   delay(1000);
   Serial.print("new IP address:\t");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.println(WiFi.softAPIP()); 
}

void loop() {
 
}
