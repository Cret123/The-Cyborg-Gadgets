// Watch 4.0: Initial Gen 4 watch, uses blinks to communicate. Chip is an ATTiny 84a.

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

const byte btn1 = 5;
const byte btn2 = 4;
const byte btn3 = 3;
const byte btn4 = 2;

const byte Func1 = 10;
const byte Func2 = 11;
const byte Func3 = 12;

volatile bool wakeup = false;

void setup() {
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(Func1, OUTPUT);
  pinMode(Func2, OUTPUT);
  pinMode(Func3, OUTPUT);
  randomSeed(analogRead(1));
}

bool button_is_pressed(const byte btn, bool onlyOnce = true) {
  if (digitalRead(btn) == LOW) {
    delay(50);
    if (onlyOnce){
      while (digitalRead(btn) == LOW){}
    }
    return true;
  }
  return false;
}

void wakeUp() {
  wakeup = true;
}

void goToSleep() {

  digitalWrite(LED_BUILTIN, LOW);

  ADCSRA &= ~(1 << ADEN);
  sleep_bod_disable();

  wakeup = false;
  attachInterrupt(digitalPinToInterrupt(btn4), wakeUp, FALLING);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(btn4));

  ADCSRA |= (1 << ADEN);
  
  if (button_is_pressed(btn4)){}
}

void activateFunc(const byte func, int blinkTime = 500) {
  bool blink = false;
  bool keepOn = false;
  while (true) {
    if (button_is_pressed(btn1, false) || keepOn) {
      digitalWrite(func, HIGH);
      delay(50);
    } else {
      digitalWrite(func, LOW);
    }

    if (blink) {
      digitalWrite(func, !digitalRead(func));
      delay(blinkTime);
    }
    digitalWrite(LED_BUILTIN, digitalRead(func));

    if (button_is_pressed(btn2))
      keepOn = !keepOn;

    else if (button_is_pressed(btn3))
      blink = !blink;

    else if (button_is_pressed(btn4))
      return;
  }
}

void watchFuncs(void) {
  while (true) {
    if (button_is_pressed(btn1)) {
      activateFunc(Func1);
      continue;
    }
    if (button_is_pressed(btn2)) {
      activateFunc(Func2);
      continue;
    }
    if (button_is_pressed(btn3)) {
      activateFunc(Func3);
      continue;
    }
    if (button_is_pressed(btn4)) {
      return;
    }
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1);
  if (button_is_pressed(btn4)){
    goToSleep();
  } 
  else if (button_is_pressed(btn1)) {
    watchFuncs();
  } 
  digitalWrite(LED_BUILTIN, LOW);
  delay(120);
}
