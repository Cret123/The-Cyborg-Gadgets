const byte BtnEnter = 2;
const byte Btn1 = 4;
const byte Btn2 = 7;
const byte Btn3 = 9;

const byte Func1 = 3;
const byte Func2 = 6;
const byte Func3 = 8;

int totalParts = 4;
int selectedPart = 1;

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

void activateFunc(const byte func, int blinkTime = 500) {
  bool blink = false;
  bool keepOn = false;
  while (true) {
    digitalWrite(LED_BUILTIN, digitalRead(func));
    if (digitalRead(Btn1) == LOW or keepOn){
      digitalWrite(func, HIGH);
      delay(50);
    }
    else
      digitalWrite(func, LOW);

    if (blink) {
      digitalWrite(func, !digitalRead(func));
      delay(blinkTime);
    }

    if (digitalRead(Btn2) == LOW)
      keepOn != keepOn;
    
    else if (digitalRead(Btn3) == LOW)
      blink = !blink;
      
    else if (digitalRead(BtnEnter) == LOW) {
      return;
    }
  }
}

void watchFuncs(void) {
  delay(50);
  while (true) {
    if (digitalRead(Btn2) == LOW){
      selectedPart++;
      if (selectedPart > totalParts) selectedPart = 1;
    } 
    else if (digitalRead(Btn1) == LOW){
      selectedPart--;
      if (selectedPart < 1) selectedPart = totalParts;
    } 
    else if (digitalRead(BtnEnter) == LOW) return;
    else if (digitalRead(Btn3) == LOW){
      switch (selectedPart) {
        case 1:
          activateFunc(Func1);
          delay(500);
          continue;
        case 2:
          activateFunc(Func2, 1000);
          continue;
        case 3:
          activateFunc(Func3);
          continue;
        case 4:
          activateFunc(LED_BUILTIN, 200);
          continue;
      }
    }
    delay(50);
  }
}

void randomInt() {
    static int clickCount = 0;
    static int prevBtn3State = HIGH;
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50;
    if (digitalRead(Btn3) == LOW){
        if (prevBtn3State == HIGH && (millis() - lastDebounceTime) > debounceDelay){
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
        return;
    }
    if (digitalRead(Btn1) == LOW){
        watchFuncs();
    }
    if (digitalRead(Btn2) == LOW){
        randomInt();
    }
}
