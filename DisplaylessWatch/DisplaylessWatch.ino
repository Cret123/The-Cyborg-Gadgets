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
