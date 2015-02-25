//
// Testtool for a small dialog between a Raspberry Pi B+ and an Arduino UNO
//
// 1) compile and deploy this sketch
// 2) connect the Arduino UNO with a USB connector to your Raspberry Pi
// 3) log-in via SSH to your Raspberry Pi
// 4) call "miniterm.py" or a similar tool, e.g. $ miniterm.py -p /dev/ttyACM0
// 5) Just enter some text...
//
// [Output Example]
// pi@raspbmc:~$ miniterm.py -p /dev/ttyACM0
// --- Miniterm on /dev/ttyACM0: 9600,8,N,1 ---
// --- Quit: Ctrl+]  |  Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
// Data: 72
// Data: 66
// Data: 76
// 
// --- exit ---
// [/Output Example]
//

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available())
  {
    byte b = Serial.read();
    
    Serial.print("Data: ");
    Serial.println(b);
  }
}
