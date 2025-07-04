const byte btn1 = 2;
const byte btn2 = 3;
const byte btn3 = 4;
const byte btn4 = 5;

const byte Func1 = A2;
const byte Func2 = A1;
const byte Func3 = 9;
const byte Func4 = 10;

int selectedFunction = 1;
const int totalFunctions = 4;

int selectedPart = 1;
const int totalParts = 4;

const int segmentPins[7] = {12, 5, 13, 10, 9, 6, 8};

const byte charSegments[] = {
  0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011, // 9
};

int charToIndex(char ch) {
  if (ch >= '0' && ch <= '9')
    return ch;
}

void displayChar(char ch) {
  int idx = charToIndex(ch);
  byte segments = (idx >= 0 && idx < sizeof(charSegments)) ? charSegments[idx] : 0;
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (segments & (1 << (6 - i))) ? HIGH : LOW);
  }
}

bool button_is_pressed(int btn) {
    if (digitalRead(btn) == LOW){
        delay(50);
        return true;
    }
    return false;
}

void activateFunc(const byte func, int blinkTime=500){
    bool blink = false;
    while (true){
    
        displayChar(digitalRead(func));

        if (blink){
            digitalWrite(func, !digitalRead(func));
            delay(blinkTime);
        }

        if (button_is_pressed(btn1))
            digitalWrite(func, HIGH);
        else
            digitalWrite(func, LOW);

        if (button_is_pressed(btn2)){
            if (digitalRead(func) == LOW)
                digitalWrite(func, HIGH);
            else 
                digitalWrite(func, LOW);
            delay(500);
        }
        else if (button_is_pressed(btn3))
            blink = !blink;
        else if (button_is_pressed(btn4)){
            digitalWrite(func, LOW);
            return;
        }
    }
}

void watchFuncs(void) {
    delay(50);
    while(true){
    
        displayChar(selectedPart);
        
        if (button_is_pressed(btn2)){
            selectedPart++;
            if (selectedPart > totalParts) selectedPart = 1;
        }
        else if (button_is_pressed(btn1)){
            selectedPart--;
            if (selectedPart < 1) selectedPart = totalParts;
        }
        else if (button_is_pressed(btn4)) return;
        else if (button_is_pressed(btn3)){
            switch (selectedPart){
                case 1:
                    activateFunc(Func1); 
                    continue;
                case 2:
                    activateFunc(Func3, 1000); 
                    continue;
                case 3:
                    activateFunc(Func4); 
                    continue;
                case 4:
                    activateFunc(LED_BUILTIN, 200); 
                    continue;
            }
        }
        delay(50);
    }
}

void randomInt(){
    int range = 1;
    while (true){
        displayChar(range);

        if (button_is_pressed(btn1)) {
            range ++;
            delay(200);
        } 
        else if (button_is_pressed(btn2)) {
            if (range > 0) range--;
            delay(200);
        } 
        else if (button_is_pressed(btn3)) {
            int randomNumber = random(0, range + 1);
            displayChar(randomNumber);
            delay(2000);
        } 
        else if (button_is_pressed(btn4)) {
            return;
        }
        delay(50);
    }
}

void bomb(){
  for (int i = 5000; i > 0; i-=20){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(i);
    digitalWrite(LED_BUILTIN, LOW);
    delay(i);
  }
}

void setup() {
  for (int i = 0; i < 7; i++)
    pinMode(segmentPins[i], OUTPUT);
  
    pinMode(btn1, INPUT_PULLUP);
    pinMode(btn2, INPUT_PULLUP);
    pinMode(btn3, INPUT_PULLUP);
    pinMode(btn4, INPUT_PULLUP);
    
    pinMode(Func1, OUTPUT);
    pinMode(Func2, OUTPUT);
    pinMode(Func3, OUTPUT);
    pinMode(Func4, OUTPUT);

    randomSeed(analogRead(7));

  for (int i = 0; i < 13; i++) {
    digitalWrite(i, HIGH);
    }
        delay(10000);


}

void loop() {

    displayChar(selectedFunction);

    if (button_is_pressed(btn2)) {
        selectedFunction++;
        if (selectedFunction > totalFunctions) selectedFunction = 1;
    }
    else if (button_is_pressed(btn1)) {
        selectedFunction--;
        if (selectedFunction < 1){
            selectedFunction = totalFunctions;
        }
    }
    else if (button_is_pressed(btn3)) {
        switch (selectedFunction) {
            case 1:
                watchFuncs(); break;
            case 2:
                randomInt(); break;
            case 3:
                bomb(); break;
            case 4:
                break;
        }
    }
    delay(20);
}
