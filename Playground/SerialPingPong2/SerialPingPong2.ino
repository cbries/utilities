void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  checkSerial();
}

#define MAX 32
char buf[MAX] = { '\0' };
byte index = 0;

void checkSerial()
{
  while (Serial.available() > 0)
  {
    char received = Serial.read();
    buf[index] = received;
    buf[index+1] = '\0';
    ++index;    
    if(index == MAX) { index = 0; }    
  }

  if(index != 0 && buf[index-1] == '\n')
  {
    Serial.print("1: "); Serial.print(buf);
    String sbuf(buf);
    Serial.print("2: "); Serial.print(sbuf);
  
    if(index == 14)
    {
      String s("");
      s = sbuf.substring(0, 2);  int x = s.toInt();
      s = sbuf.substring(2, 4);  int y = s.toInt();
      s = sbuf.substring(4, 7);  int RED = s.toInt();
      s = sbuf.substring(7, 10);  int GREEN = s.toInt();
      s = sbuf.substring(10, 13); int BLUE = s.toInt();
      
      Serial.print("x: "); Serial.println(x);
      Serial.print("y: "); Serial.println(y);
      Serial.print("R: "); Serial.println(RED);
      Serial.print("G: "); Serial.println(GREEN);
      Serial.print("B: "); Serial.println(BLUE);
      Serial.println("----------");
    }
    
    index = 0;
  }
}

