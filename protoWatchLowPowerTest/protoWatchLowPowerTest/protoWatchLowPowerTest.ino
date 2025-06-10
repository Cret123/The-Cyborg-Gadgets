#include "ArduinoLowPower.h"

const byte BtnEnter = 2;
const byte Btn1 = 4;
const byte Btn2 = 7;
const byte Btn3 = 9;

const byte Func1 = 3;
const byte Func2 = 6;
const byte Func3 = 8;

volatile bool isSleeping = false;
volatile bool wakeUpFlag = false;

void setup()
{
    pinMode(BtnEnter, INPUT_PULLUP);
    pinMode(Btn1, INPUT_PULLUP);
    pinMode(Btn2, INPUT_PULLUP);
    pinMode(Btn3, INPUT_PULLUP);
    pinMode(Func1, OUTPUT);
    pinMode(Func2, OUTPUT);
    pinMode(Func3, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    attachInterrupt(digitalPinToInterrupt(BtnEnter), wakeUp, CHANGE);
}

void wakeUp() {
    wakeUpFlag = true;
}

void watchFuncs()
{
  while (true){
    if (digitalRead(Btn1) == LOW){
      digitalWrite(Func1, HIGH);
    }
    if (digitalRead(Btn2) == LOW){
      digitalWrite(Func2, HIGH);
    }
    if (digitalRead(Btn3) == LOW){
      digitalWrite(Func3, HIGH);
    }
    // Small delay to debounce buttons
    delay(50);
  }
}

void loop()
{
  static bool isAwake = true; // Track awake/sleep state

  if (isAwake) {
    // Normal operation
    digitalWrite(LED_BUILTIN, HIGH);

    if (digitalRead(BtnEnter) == LOW) {
      // Debounce
      delay(50);
      if (digitalRead(BtnEnter) == LOW) {
        // Enter low power mode
        isAwake = false;
        isSleeping = true;
        // Detach interrupt before sleeping to prevent multiple triggers
        detachInterrupt(digitalPinToInterrupt(BtnEnter));
        // Optional: turn off outputs or do other shutdown tasks here

        // Enter sleep mode, will wake on BtnEnter interrupt
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
        // When woke up, execution resumes here
        // Re-attach interrupt
        attachInterrupt(digitalPinToInterrupt(BtnEnter), wakeUp, CHANGE);
        isSleeping = false;
      }
    }

    // Check if wake-up flag was set via interrupt (should be redundant here)
    if (wakeUpFlag) {
      wakeUpFlag = false;
      // Wake-up actions if needed
    }

    if (digitalRead(Btn1) == LOW){
          watchFuncs()
          continue
      }
      if (digitalRead(BtnEnter) == LOW){
          watchFuncs()
          continue
      }
  } else {
    continue
  }
}
