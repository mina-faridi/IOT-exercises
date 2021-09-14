#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5;     
constexpr uint8_t SS_PIN = 4;    

 
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key; 


byte nuidPICC[4];

void setup() { 
  Serial.begin(9600);
  SPI.begin(); 
  rfid.PCD_Init();  
  pinMode(D4, OUTPUT);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
 
void loop() {

  
  if ( ! rfid.PICC_IsNewCardPresent())
    return;


  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

 
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));


    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));

    if(rfid.uid.uidByte[0]== 185){
    Serial.println(">>..... this is rfid card tag ....<");
    digitalWrite(D4, HIGH);
    }
    
    if(rfid.uid.uidByte[0]== 58){
    Serial.println(">>.... this is rfid key fob ....<<");
    digitalWrite(D4, LOW);
  }
  
    Serial.print(F("In hex: "));  
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

  
  rfid.PICC_HaltA();


  rfid.PCD_StopCrypto1();
}


void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
  Serial.print(buffer[i] < 0x10 ? " 0" : " ");
  Serial.print(buffer[i], HEX);
  }
}


void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
