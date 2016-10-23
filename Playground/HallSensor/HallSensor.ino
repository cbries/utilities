/*
 Hall effect sensor used is the A1120EUA from Allegro Microsystems 
 http://www.hobbytronics.co.uk/arduino-tutorial8-hall-effect
*/

#include <EEPROM.h>
#include "EepromUtil.h"

const int hallPin = A0;     
const int ledPin =  13;     

const long eepromAddressCounter = 0;
const long eepromAddressLastAccess = 0x0A;

#define MAX_LENGTH 18
char lastAccess[MAX_LENGTH] = {'\0'};
int hallStateCounter = 0;
int hallState = 0;
bool hallStateChanged = false;

void setup() 
{
  pinMode(ledPin, OUTPUT);      
  pinMode(hallPin, INPUT);     
  //Serial.begin(9600);
  Serial2.begin(9600);
  EepromUtil::eeprom_read_int(eepromAddressCounter, &hallStateCounter);
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

  Serial2.print("{\"counter\":");
  Serial2.print(hallStateCounter);
  Serial2.print(",\"lastAccess\":\"");
  Serial2.print(lastAccess);
  Serial2.print("\"");
  Serial2.println("}");

  checkSerial();
}

void checkSerial()
{
  if(Serial2.available() > 0)
  {
    byte dataInLength = Serial2.available();
    if(dataInLength <= 2)
    {
      byte dataIn = Serial2.read();
      if(dataIn == 'r' /* reset counter */)
      {
        hallStateCounter = 0x00;
        EepromUtil::eeprom_write_int(eepromAddressCounter, hallStateCounter);
      }
    }
    else
    {
      String s = Serial2.readString();
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

