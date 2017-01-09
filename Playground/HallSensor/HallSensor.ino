/*
 Hall effect sensor used is the A1120EUA from Allegro Microsystems 
 http://www.hobbytronics.co.uk/arduino-tutorial8-hall-effect
*/

#include <EEPROM.h>
#include "EepromUtil.h"

const int hallPin = A0;
const int irPin = A1; 
const int ledPin =  13;

const long eepromAddressCounter = 0x00;
const long eepromAddressCounter2 = 0x04;
const long eepromAddressLastAccess = 0x0A;

#define MAX_LENGTH 18
char lastAccess[MAX_LENGTH] = {'\0'};
int irStateCounter = 0;
int hallStateCounter = 0;
int hallState = 0;
bool hallStateChanged = false;

void setup() 
{
  pinMode(ledPin, OUTPUT);      
  pinMode(hallPin, INPUT);     
  pinMode(irPin, INPUT);
  Serial.begin(9600);
  //Serial2.begin(9600);
  EepromUtil::eeprom_read_int(eepromAddressCounter, &hallStateCounter);
  EepromUtil::eeprom_read_int(eepromAddressCounter2, &irStateCounter);
  EepromUtil::eeprom_read_string(eepromAddressLastAccess, lastAccess, MAX_LENGTH);
}

void loop()
{
  int currentHallState = digitalRead(hallPin);
  hallStateChanged = hallState != currentHallState;
  hallState = currentHallState;

  if (hallState == LOW)
    digitalWrite(ledPin, HIGH);  
  else 
    digitalWrite(ledPin, LOW);

  if(hallStateChanged)
  {
    if(hallState == LOW)
    {
      ++hallStateCounter;

      EepromUtil::eeprom_write_int(eepromAddressCounter, hallStateCounter);
    }
  }

  // ir stuff
  int irRead = analogRead(irPin);

  // send via bluetooth
  Serial.print("{\"counter\":");
  Serial.print(hallStateCounter);
  Serial.print(",\"ir\":");
  Serial.print(irRead);
  Serial.print(",\"lastAccess\":\"");
  Serial.print(lastAccess);
  Serial.print("\"");
  Serial.println("}");
  
  checkSerial();
}

void checkSerial()
{
  if(Serial.available() > 0)
  {
    byte dataInLength = Serial.available();
    if(dataInLength <= 2)
    {
      byte dataIn = Serial.read();
      if(dataIn == 'r' /* reset counter */)
      {
        hallStateCounter = 0x00;
        EepromUtil::eeprom_write_int(eepromAddressCounter, hallStateCounter);
      }
    }
    else
    {
      String s = Serial.readString();
      String lastAccessCmd = getValue(s, ':', 0);
      String lastAccessValue = getValue(s, ':', 1);

      if(lastAccessCmd == "access")
      {
        // DD/MM/YY,HH:MM:SS
        // e.g. "29/06/16,20-12-32"
        // -> max length: 17
        lastAccessValue.toCharArray(lastAccess, MAX_LENGTH);
        EepromUtil::eeprom_write_string(eepromAddressLastAccess, lastAccess);
      }
    }
  }
}

// ################################################################
//    U T I L I T I E S
// ################################################################

String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

