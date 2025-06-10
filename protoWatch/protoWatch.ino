#include <LowPower.h>

const byte BtnEnter = 2;
const byte Btn1 = 4;
const byte Btn2 = 7;
const byte Btn3 = 9;

const byte Func1 = 3;
const byte Func2 = 6;
const byte Func3 = 8;

void setup()
{
    pinMode(BtnEnter, INPUT_PULLUP);
    pinMode(Btn1, INPUT_PULLUP);
    pinMode(Btn2, INPUT_PULLUP);
    pinMode(Btn3, INPUT_PULLUP);
    pinMode(Func1, OUTPUT);
    pinMode(Func2, OUTPUT);
    pinMode(Func3, OUTPUT);
    randomSeed(analogRead(1));
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void watchFuncs(void)
{
    digitalWrite(LED_BUILTIN, LOW);
    while (true){
        if (digitalRead(BtnEnter) == LOW){
            digitalWrite(LED_BUILTIN, HIGH);
            return;
        }
        while (digitalRead(Btn1) == LOW){
            digitalWrite(Func1, HIGH);
            digitalWrite(LED_BUILTIN, HIGH);
        }
        digitalWrite(Func1, LOW);

        while (digitalRead(Btn2) == LOW){
            digitalWrite(Func2, HIGH);
            digitalWrite(LED_BUILTIN, HIGH);
        }
        digitalWrite(Func2, LOW);

        while (digitalRead(Btn3) == LOW){
            digitalWrite(Func3, HIGH);
            digitalWrite(LED_BUILTIN, HIGH);
        }
        digitalWrite(Func3, LOW);
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void randomInt() {
    static int clickCount = 0;
    static int prevBtn3State = HIGH;
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50;
    if (digitalRead(Btn3) == LOW) {
        if (prevBtn3State == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
            clickCount++;
            lastDebounceTime = millis();
        }
        prevBtn3State = LOW;
    } else {
        prevBtn3State = HIGH;
    }
    if (digitalRead(Btn1) == LOW) {
        int range = clickCount;
        int randNumber = random(0, range + 1);
        for (int i = 0; i < randNumber; i++){
            digitalWrite(LED_BUILTIN, LOW);
            delay(250);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(250);
        }
    }
})

void simpleNumberCalculator() {
  static int num1 = 0;
  static int num2 = 0;
  static int operatorIndex = 0; // 0: +, 1: -, 2: *, 3: /
  static int lastBtn1State = HIGH;
  static int lastBtn2State = HIGH;
  static int lastBtn3State = HIGH;
  static int lastBtnEnterState = HIGH;

  // Read current button states
  int btn1State = digitalRead(Btn1Pin);
  int btn2State = digitalRead(Btn2Pin);
  int btn3State = digitalRead(Btn3Pin);
  int btnEnterState = digitalRead(BtnEnterPin);

  // Detect button presses (assuming active LOW)
  if (lastBtn1State == HIGH && btn1State == LOW) {
    num1++;
  }

  if (lastBtn3State == HIGH && btn3State == LOW) {
    num2++;
  }

  if (lastBtn2State == HIGH && btn2State == LOW) {
    // Cycle through operators: +, -, *, /
    operatorIndex = (operatorIndex + 1) % 4;
  }

  if (lastBtnEnterState == HIGH && btnEnterState == LOW) {
    // Perform calculation
    float result;
    switch (operatorIndex) {
      case 0: // addition
        result = num1 + num2;
        break;
      case 1: // subtraction
        result = num1 - num2;
        break;
      case 2: // multiplication
        result = num1 * num2;
        break;
      case 3: // division
        if (num2 != 0) {
          result = (float)num1 / num2;
        } else {
          // Handle division by zero, blink LED in error pattern
          for (int i = 0; i < 3; i++) {
            digitalWrite(LedPin, HIGH);
            delay(300);
            digitalWrite(LedPin, LOW);
            delay(300);
          }
          return;
        }
        break;
    }
    // Blink the result (convert to integer for blinking count)
    int blinkCount = (int)abs(result);
    if (blinkCount == 0) blinkCount = 1; // ensure at least one blink

    for (int i = 0; i < blinkCount; i++) {
      digitalWrite(LedPin, HIGH);
      delay(300);
      digitalWrite(LedPin, LOW);
      delay(300);
    }
  }

  // Save current states for edge detection
  lastBtn1State = btn1State;
  lastBtn2State = btn2State;
  lastBtn3State = btn3State;
  lastBtnEnterState = btnEnterState;
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    if (digitalRead(BtnEnter) == LOW){
        if (digitalRead(BtnEnter) == LOW);
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    }
    if (digitalRead(Btn1) == LOW){
        watchFuncs();
    }
    if (digitalRead(Btn2) == LOW){
        randomInt();
    }
}
