#include <X10Firecracker.h>

#define RTS_PIN     7                   // RTS line for C17A - DB9 pin 7
#define DTR_PIN     4                   // DTR line for C17A - DB9 pin 4

void setup()
{
  X10.init( RTS_PIN, DTR_PIN );
  pinMode(13, OUTPUT);
}

void on()
{
  X10.sendCmd( hcN, 2, cmdOn );
}

void off()
{
  X10.sendCmd( hcN, 2, cmdOff );
}

void loop()
{
  off();
  digitalWrite(13, HIGH);   // set the LED on
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(500);              // wait for a second
  digitalWrite(13, HIGH);   // set the LED on
  delay(500);              // wait for a second
  on();
  digitalWrite(13, LOW);    // set the LED off
  delay(500);              // wait for a second
  digitalWrite(13, HIGH);   // set the LED on
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(500);              // wait for a second
}
