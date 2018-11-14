#include "U8glib.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

#define BEEP 14

#define SSID ""
#define PASS ""
#define DST_IP ""
boolean connectedToWlan = false;

Adafruit_PN532 nfc(8, 5, 21, 4); //PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

uint8_t success;                          // Flag to check if there was an error with the PN532
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
String cardUID;
char responseFromServer[200];
int responsePos;
String drinkLog = "";
String cmd;

void setup() {  
  u8g.setFont(u8g_font_7x14);
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on.
  
  Serial.begin(115200);

  drawLog("Firing up NFC");
  nfc.begin();
  
  delay(1000);
  nfc.SAMConfig();
  
  pinMode(BEEP, OUTPUT);
  
  Serial1.begin(9600);
  Serial1.setTimeout(15000);

  cardUID.reserve(32);
  drinkLog.reserve(150);
  cmd.reserve(128);

  refreshESPInfo();
}

void loop() {  
  if (!connectedToWlan) {
    connectToNetwork();
  }
  else {
    if (drinkLog.length() == 0) {
      refreshLog();
    }
    
    u8g.firstPage();
    do {  
      draw();
    } while( u8g.nextPage() );
    
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    
    if (success) {
      cardUID = "";
      for (uint32_t szPos=0; szPos < uidLength; szPos++)
      {
        cardUID += String(uid[szPos]);
      }
      u8g.firstPage();  
      do {
        drawCardInfo();
      } 
      while(u8g.nextPage());
      sendData(cardUID);
      drawSuccess(responseFromServer);
      drinkLog = "";
    }  
    
    delay(5000);   
  }
}

void flushSerial() {
  while (Serial1.available()) {
    Serial1.read();
  }
}
