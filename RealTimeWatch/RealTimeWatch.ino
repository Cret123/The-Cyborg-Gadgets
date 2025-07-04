#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ctype.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int selectedFunction = 1;
const int totalFunctions = 4;

int selectedPart = 1;
const int totalParts = 4;

const byte btn1 = 6;
const byte btn2 = 4;
const byte btn3 = 3;
const byte btn4 = 5;

const byte Func1 = 7;
const byte Func2 = 8;
const byte Func3 = 9;
const byte Func4 = 10;

volatile bool wakeup = false;

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(5, 0);
    display.println("Wecome to");
    display.setCursor(10, 35);
    display.setTextSize(2);
    display.println("Watch 10!");
    display.display();
    
    pinMode(btn1, INPUT_PULLUP);
    pinMode(btn2, INPUT_PULLUP);
    pinMode(btn3, INPUT_PULLUP);
    pinMode(btn4, INPUT_PULLUP);
    pinMode(Func1, OUTPUT);
    pinMode(Func2, OUTPUT);
    pinMode(Func3, OUTPUT);
    pinMode(Func4, OUTPUT);
    randomSeed(analogRead(1));
    
    for(int i=0; i<100; i++){
        delay(50);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        if (button_is_pressed(btn3)){
            digitalWrite(Func1, HIGH);  
        }
        else if (button_is_pressed(btn2)){
            digitalWrite(Func3, HIGH);
        }
    }
}

bool button_is_pressed(int btn) {
    if (digitalRead(btn) == LOW){
        delay(50);
        return true;
    }
    return false;
}

void wakeUp() {
    wakeup = true;
}

void activateFunc(const byte func, int blinkTime=500){
    bool blink = false;
    while (true){
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 20);
        display.println("1. Quick Flash");
        display.setCursor(0, 30);
        display.println("2. Always On");
        display.setCursor(0, 40);
        display.println("3. Blink");
        display.setCursor(0, 50);
        display.println("4. Return");
        
        display.setTextSize(2);
        display.setCursor(10, 0);
        display.print("State: ");
        display.print(digitalRead(func));
        display.display();

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
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 20);
        display.println("1. White LED");
        display.setCursor(0, 30);
        display.println("2. Laser");
        display.setCursor(0, 40);
        display.println("3. UV LED");
        display.setCursor(0, 50);
        display.println("4. BuiltIn LED");
        display.setTextSize(2);
        display.setCursor(10, 0);
        display.print("Sel: ");
        display.print(selectedPart);
        display.display();
        
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
                    activateFunc(Func1); continue;
                case 2:
                    activateFunc(Func3, 1000); continue;
                case 3:
                    activateFunc(Func4); continue;
                case 4:
                    activateFunc(LED_BUILTIN, 200); continue;
            }
        }
        delay(50);
    }
}

void calculator(void) {
    const char* options[] = {"1","2","3","4","5","6","7","8","9","0",".","+","-","*","/","^","r","%","(",")"};
    const int numOptions = sizeof(options)/sizeof(options[0]);
    int currentOption = 0;
    char expr[25] = "";
    int exprLen = 0;
    bool done = false;
    bool showResult = false;
    double result = 0;
    bool error = false;

    while (!done) {
        display.clearDisplay();
        
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.print("Calculator");

        display.setTextSize(1);
        display.setCursor(0, 24);
        display.print(expr);

        int startX = 30;
        int yPos = 20;
        int spacing = 12;

        for (int i = 0; i < numOptions; i++) {
            int x = startX + (i % 7) * spacing;
            int y = yPos + (i / 7) * 10;
            if (i == currentOption) {
                display.fillRect(x-1, y-1, spacing+2, 10, SSD1306_WHITE);
                display.setTextColor(SSD1306_BLACK);
                display.setCursor(x, y);
                display.print(options[i]);
                display.setTextColor(SSD1306_WHITE);
            } else {
                display.setCursor(x, y);
                display.print(options[i]);
            }
        }

        if (showResult) {
            if (error) {
                display.setCursor(10, 48);
                display.print("Error!");
            } else {
                display.setCursor(0, 56);
                display.print("= ");
                display.print(result, 6);
            }
        }

        display.display();

        if (button_is_pressed(btn1)) {
            currentOption = (currentOption + 1) % numOptions;
            delay(200);
        }
        else if (button_is_pressed(btn2)) {
            if (exprLen < 24) {
                strcat(expr, options[currentOption]);
                exprLen = strlen(expr);
            }
            showResult = false;
            delay(200);
        }
        else if (button_is_pressed(btn3)) {
            error = false;
            result = evaluateExpression(expr, error);
            showResult = true;
            delay(500);
        }
        else if (button_is_pressed(btn4)) {
            delay(200);
            return;
        }
        delay(50);
    }
}

double parsePrimary(const char* &s, bool &error);

double parseNumber(const char* &s, bool &error) {
    double value = 0.0;
    bool hasDecimal = false;
    double frac = 0.1;
    while (isdigit(*s) || *s == '.') {
        if (*s == '.') {
            if (hasDecimal) { error = true; return 0; }
            hasDecimal = true;
        } else if (hasDecimal) {
            value += (*s - '0') * frac;
            frac *= 0.1;
        } else {
            value = value * 10 + (*s - '0');
        }
        s++;
    }
    return value;
}

double parseFactor(const char* &s, bool &error) {
    while (*s == ' ') s++;
    if (*s == '+') { s++; return parseFactor(s, error); }
    if (*s == '-') { s++; return -parseFactor(s, error); }
    if (*s == 'r') { s++; double val = parseFactor(s, error); return val<0?(error=true,0):sqrt(val); }
    if (*s == '(') {
        s++;
        double val = parsePrimary(s, error);
        if (*s == ')') s++;
        else error = true;
        return val;
    }
    return parseNumber(s, error);
}

double parseExponent(const char* &s, bool &error) {
    double left = parseFactor(s, error);
    while (*s == '^') {
        s++;
        double right = parseFactor(s, error);
        left = pow(left, right);
    }
    return left;
}

double parseTerm(const char* &s, bool &error) {
    double left = parseExponent(s, error);
    while (*s == '*' || *s == '/' || *s == '%') {
        char op = *s++;
        double right = parseExponent(s, error);
        if (op == '*') left *= right;
        else if (op == '/') { if (right == 0) { error = true; return 0; } left /= right; }
        else if (op == '%') { if (right == 0) { error = true; return 0; } left = fmod(left, right); }
    }
    return left;
}

double parsePrimary(const char* &s, bool &error) {
    double left = parseTerm(s, error);
    while (*s == '+' || *s == '-') {
        char op = *s++;
        double right = parseTerm(s, error);
        if (op == '+') left += right;
        else left -= right;
    }
    return left;
}

double evaluateExpression(const char* expr, bool &error) {
    const char* s = expr;
    error = false;
    double result = parsePrimary(s, error);
    while (*s && !error) {
        if (!isspace(*s)) error = true;
        s++;
    }
    return result;
}

void randomInt() {
    int range = 1;
    bool inRandomInt = true;
    while (inRandomInt) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println("Random Int");
        display.setCursor(10, 32);
        display.print("Range: 0 - ");
        display.print(range);
        display.display();

        if (button_is_pressed(btn1)) {
            range += 10;
            delay(200);
        } 
        else if (button_is_pressed(btn2)) {
            if (range > 0) range--;
            delay(200);
        } 
        else if (button_is_pressed(btn3)) {
            int randNumber = random(0, range + 1);
            display.clearDisplay();
            display.setTextSize(2);
            display.setCursor(10, 24);
            display.print("Rnd: ");
            display.print(randNumber);
            display.display();
            delay(2000);
        } 
        else if (button_is_pressed(btn4)) {
            inRandomInt = false;
        }
        delay(50);
    }
}

void loop() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Menu     ");
    display.print(selectedFunction);

    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print("1. Output   2. Maths");
    display.setCursor(0, 40);
    display.print("3. Random   4. Blink");
    display.setCursor(10, 48);

    display.display();

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
                calculator(); break;
            case 3:
                randomInt(); break;
            case 4:
                break;
        }
    }
    delay(20);
}
