/* Firmware for a selfmade Rowing-machine
 * Author: Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Date: January, 2017
 * 
 * Some parts are taken :)
 * [1] http://forum.zerspanungsbude.net/viewtopic.php?t=1601
 */

/*
 * Communication
 * + Message Format to reset current states:
 *   a connected device must send 'r' to reset any counters
 * + Message Format to any receiver:
 *   {\r\n                   3 Bytes
 *   "time":%10lu,\r\n      20 Bytes  
 *   "rpm":%5lu,\r\n        14 Bytes
 *   "pulls":%5lu,\r\n      16 Bytes
 *   "rotations":%10lu\r\n  24 Bytes
 *   }                       1 Byte
 *   --------------------------------
 *                        > 78 Bytes  (NrOfBufferBytes)
 */
const byte NrOfBufferBytes = 128;

const byte PullInput = 3;
const byte rpmInput = 2;
const byte RpmAverage = 4;

const bool SendMode = 0;  // 0:=serial, 1:=bluetooth

const byte BluetoothRxPin = 10;
const byte BluetoothTxPin = 11;

// #########################################################################

#include <SoftwareSerial.h>

volatile unsigned long _duration = 0;
volatile unsigned long _durationPull = 0;
volatile unsigned long _last = 0;
volatile unsigned long _lastPull = 0;
volatile unsigned long _average = 1;
volatile int _avgcnt=0;            

char _buf[NrOfBufferBytes];
unsigned long _rotations = 0;
unsigned long _rpms = 0;
int _rowingPulling = 0;
byte _rowingPullCount = 0;

SoftwareSerial bluetoothSerial(BluetoothRxPin, BluetoothTxPin);

void(*resetFunc) (void) = 0;

void setup() 
{
  if(!SendMode)
    Serial.begin(9600);
  else
    bluetoothSerial.begin(9600);
  delay(300);
  pinMode(PullInput, INPUT);
  digitalWrite(PullInput, HIGH);  
  pinMode(rpmInput, INPUT);
  digitalWrite(rpmInput, HIGH);  
  delay(300);
  attachInterrupt(0, readmicros, RISING );
  attachInterrupt(1, rowingPull, RISING );   
}

void checkReset()
{
  byte data = '\0';
  
  if(!SendMode)
  {
    if(Serial.available())
      data = Serial.read();
    else
      return;
  }
  else
  {
    if(bluetoothSerial.available())
      data = Serial.read();
    else
      return;
  }
  
  if(data == 'r')
  {
    resetFunc();
  }
}

void createMessage(char buf[])
{
  _buf[0] = '\0';
  sprintf(buf, "{\r\n\"time\":%10lu,\r\n\"rpm\":%5lu,\r\n\"pulls\":%5lu\r\n,\"rotations\":%10lu\r\n}",
    millis(), _rpms, _rowingPulling, _rotations); 
}

void loop() 
{
  if (_duration != 0) 
  {
    _rpms = myround(60000000 / _average);
  } 
  else
  {
    _rpms = 0;
    _avgcnt = 0;
  }  

  if(_rowingPullCount >= 2)
  {
    _rowingPullCount = 0;
    ++_rowingPulling;
  }

  createMessage(_buf);
  if(!SendMode)
    Serial.println(_buf);
  else
    bluetoothSerial.println(_buf);

  _duration >>= 10;
  
  delay(125);
}

void rowingPull()
{
  detachInterrupt(1);
  unsigned long us = micros();
  if(_lastPull == 0)
  {
    _lastPull = us;
  }
  else 
  {
    if(us < _lastPull) 
    {
      _durationPull = 4294967295 - _lastPull + us;
    } 
    else 
    {
      _durationPull = us - _lastPull;
    }   
  }
  if(_durationPull > 5000) 
  {
    ++_rowingPullCount;
    _lastPull = us;
  }
  attachInterrupt(1, rowingPull, RISING );    
}

void readmicros() 
{
  detachInterrupt(0);
  unsigned long us = micros();
  int avgmax;
  if (_last == 0) 
  {  
    _last = us;
  } 
  else 
  { 
    if (us < _last) 
    {
      _duration = 4294967295 - _last + us;
    } 
    else 
    {
      _duration = us - _last;
    }
    
    if (_duration > 5000) 
    {
      ++_rotations;
      
      _average = _duration + _average * _avgcnt++; 
      _average /= _avgcnt;
      avgmax = 1000000 / _duration;
      
      if (avgmax < RpmAverage) 
        avgmax = RpmAverage;
      
      if (_avgcnt >= avgmax) 
        _avgcnt--;
        
      _last = us;
    }
  }
  attachInterrupt(0, readmicros, RISING);
}

unsigned long myround(unsigned long value) {
  int rto;
  if (value > 3000) {
    rto = 100;
  } else if (value > 1500) {
    rto = 50;
  } else if (value > 500) {
    rto = 10;
  } else if (value > 100) {
    rto = 5;
  } else {
    return (value);
  }
  return (_myround(value, rto));
}

unsigned long _myround(unsigned long value, int roundto) {
  value += (roundto >> 1);
  value /= roundto;  
  value *= roundto;
  return (value);
}

