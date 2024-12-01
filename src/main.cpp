#include <Arduino.h>

#define ledsA PIN_PA6
#define ledsB PIN_PA3
#define ledsC PIN_PA2
#define ledD PIN_PA1
#define touch PIN_PA7

const int16_t countValue[10] PROGMEM = { // organized in value, step, where value is mSec, step number of step to get to the next value
    10, 200,
    50, 10,
    100, 5,
    500, 1};

void setup()
{
  pinMode(ledsA, OUTPUT);
  pinMode(ledsB, OUTPUT);
  pinMode(ledsC, OUTPUT);
  pinMode(ledD, OUTPUT);
  pinMode(touch, INPUT);
}

void allOff()
{
  digitalWrite(ledsA, 0);
  digitalWrite(ledsB, 0);
  digitalWrite(ledsC, 0);
  digitalWrite(ledD, 0);
}

void setValue(uint8_t value)
{
  allOff();
  switch (value)
  {
  case 1:
    digitalWrite(ledD, 1);
    return;
  case 2:
    digitalWrite(ledsA, 1);
    return;
  case 3:
    digitalWrite(ledsC, 1);
    digitalWrite(ledD, 1);
    return;
  case 4:
    digitalWrite(ledsC, 1);
    digitalWrite(ledsA, 1);
    return;
  case 5:
    digitalWrite(ledsC, 1);
    digitalWrite(ledsA, 1);
    digitalWrite(ledD, 1);
    return;
  case 6:
    digitalWrite(ledsC, 1);
    digitalWrite(ledsB, 1);
    digitalWrite(ledsA, 1);
    return;
  }
}

uint16_t wait = 150;
bool WaitLoop(bool init = false)
{
  if (init)
    wait = 150;

  uint16_t delayValue = 1200 / wait--;
  delay(delayValue);

  if (!wait)
    return true;
  return false;
}

uint16_t lfsr = 0x3CE1; // seed
uint16_t bit;           // Holds the feedback bit

uint16_t lfsr_next() // pseudo random lfsr 16bit
{
  bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
  lfsr = (lfsr >> 1) | (bit << 15);
  return lfsr;
}

uint8_t value;
void loop()
{
  while (!digitalRead(touch)) // hold while not pressed
  {
    delay(10);
    lfsr_next();  // keep randomizing...
  }

  while (digitalRead(touch)) // touch pressed...
  {
    value = lfsr_next() % 6 + 1;
    setValue(value);
  }

  WaitLoop(true); // touch released, animate runout
  while (!WaitLoop())
  {
    value++;
    if (value > 6)
      value = 1;
    setValue(value);
  }

  for (uint8_t count = 0; count < 6; count++) // blink value
  {
    delay(200);
    setValue(0);
    delay(200);
    setValue(value);
  }

  delay(3000);    // holdoff 3 secs
}
