// Watch 4.1: Initial Gen 4 watch, uses blinks to communicate. Chip is an ATTiny 84a.

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define KEEPON_THRESHOLD 10

const byte onPin = 0;
const byte offPin = 0;
const byte outputs = 0;

volatile bool wakeup = false;

void setup() {
  pinMode(onPin, INPUT);
  pinMode(offPin, INPUT);
  pinMode(outputs, OUTPUT);
}

void loop(){
  blinkOn = analogRead(onPin);
  blinkOff = analogRead(offPin);
  if (blinkOff > KEEPON_THRESHOLD){
    digitalWrite(outputs, HIGH);
    delay(blinkOn);
    digitalWrite(outputs, LOW);
    delay(blinkOff);
  }
  else digitalWrite(outputs, HIGH);
}
