// Initial arduino pro mini watch

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

unsigned long lastActivityTime = 0;
const unsigned long inactivityPeriod = 30000;

const byte btn1 = 4;
const byte btn2 = 7;
const byte btn3 = 9;
const byte btn4 = 2;

const byte Func1 = 3;
const byte Func2 = 6;
const byte Func3 = 8;

int totalParts = 4;
int selectedPart = 1;

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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

bool button_is_pressed(const byte btn) {
  unsigned long now = millis();

  if (now - lastActivityTime > inactivityPeriod) {
    goToSleep();
    lastActivityTime = millis();
    // Ensure interrupt does not trigger immediately
    while (button_is_pressed(btn3))
      ;
    return false;
  }

  if (digitalRead(btn) == LOW) {
    delay(50);
    lastActivityTime = millis();
    return true;
  }
  return false;
}

void (*reset)(void) = 0;

void wakeUp() {
  wakeup = true;
}

void goToSleep() {

  ADCSRA &= ~(1 << ADEN);
  sleep_bod_disable();

  wakeup = false;
  attachInterrupt(digitalPinToInterrupt(btn3), wakeUp, FALLING);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(btn3));

  ADCSRA |= (1 << ADEN);
}

void activateFunc(const byte func, int blinkTime = 500) {
  bool blink = false;
  bool keepOn = false;
  while (true) {
    digitalWrite(LED_BUILTIN, digitalRead(func));
    if (button_is_pressed(btn1) or keepOn) {
      digitalWrite(func, HIGH);
      delay(50);
    } else
      digitalWrite(func, LOW);

    if (blink) {
      digitalWrite(func, !digitalRead(func));
      delay(blinkTime);
    }

    if (button_is_pressed(btn2))
      keepOn != keepOn;

    else if (button_is_pressed(btn3))
      blink = !blink;

    else if (button_is_pressed(btn4)) {
      return;
    }
  }
}

void watchFuncs(void) {
  delay(50);
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

void randomInt() {
  static int clickCount = 0;
  static int prevBtn3State = HIGH;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;
  if (button_is_pressed(btn3)) {
    if (prevBtn3State == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
      clickCount++;
      lastDebounceTime = millis();
    }
    prevBtn3State = LOW;
  } else {
    prevBtn3State = HIGH;
  }
  if (button_is_pressed(btn1)) {
    int range = clickCount;
    int randNumber = random(0, range + 1);
    for (int i = 0; i < randNumber; i++) {
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
    }
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  if (button_is_pressed(btn4)) {
    while (button_is_pressed(btn4))
      ;
    goToSleep();
  } else if (button_is_pressed(btn1)) {
    watchFuncs();
  } else if (button_is_pressed(btn2)) {
    randomInt();
  } else if (button_is_pressed(btn3)) {
    reset();
  }
}