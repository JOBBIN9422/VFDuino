#include <VfdClock.h>

VfdClock vfdClock;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.print("test");
}

void loop() {
  vfdClock.displayTime();
//  for (int i = 1; i < 9; i++)
//  {
//    vfdClock.setGrid(i);
//    for (byte d = 0; d < 10; d++)
//    {
//      vfdClock.setDigit(d);
//      delay(500); 
//    }
//    delay(500);
//  }
}
