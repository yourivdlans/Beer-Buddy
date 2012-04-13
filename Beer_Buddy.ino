#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

#include <SoftwareSerial.h>

#include <BeerBuddyRfid.h>
#include <BeerBuddyEthernet.h>
#include <BeerBuddyUser.h>
#include <BeerBuddyDraft.h>

#define RED 8
#define GREEN 7

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x03, 0x05 };
IPAddress ip(85,214,92,47);
char host[] = "http://beer-buddy.nl";

BeerBuddyRfid rfid(2, 3);
BeerBuddyEthernet ethernet(mac, ip, host);
BeerBuddyDraft draft(6, 5);

bool cardPresent;
char* card;
int enableLed = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  
  ethernet.initialize();
  ethernet.setOnline();
  
  draft.setupServos();
}

void checkForCard()
{
  cardPresent = rfid.checkCard();
  
  if ( cardPresent == true )
  {
    Serial.println(cardPresent);
    
    card = rfid.getCard();
    
    Serial.println(card);
    
    if ( ethernet.sendRFID(card) )
    {
      enableLed = GREEN;
      
      draft.draftBeer();
    }
    else
    {
      enableLed = RED;
    }
    
    cardPresent = false;
  }
}

void checkForLedEnable()
{
  if ( enableLed != 0 )
  {
    if ( !rfid.setLed(enableLed, 1000) )
    {
      enableLed = 0;
    }
  }
}

void loop()
{
  //ethernet.enableKeepAlive();
  
  checkForCard();
  checkForLedEnable();
}
