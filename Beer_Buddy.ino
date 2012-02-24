#include <SPI.h>
#include <Ethernet.h>

#include <SoftwareSerial.h>

#include <BeerBuddyRfid.h>
#include <BeerBuddyEthernet.h>

#define RED 8
#define GREEN 7

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x03, 0x05 };
IPAddress ip(85,214,92,47);
char host[] = "http://beer-buddy.nl";

BeerBuddyRfid rfid(2, 3);
BeerBuddyEthernet ethernet(mac, ip, host);

bool cardPresent;

void setup()
{
  Serial.begin(9600);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  
  ethernet.initialize();
  ethernet.setOnline();
}

void loop()
{
  ethernet.enableKeepAlive();
  
  cardPresent = rfid.checkCard();
  
  if ( cardPresent == true )
  {
    Serial.println(cardPresent);
    
    char* card = rfid.getCard();
    
    Serial.println(card);
    
    ethernet.sendRFID(card);
    
    cardPresent = false;
  }
}
