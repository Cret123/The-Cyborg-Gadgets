#include <U8g2lib.h>
#include <ctype.h>
#include <math.h>

U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

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

void setup()
{
    pinMode(btn1, INPUT_PULLUP);
    pinMode(btn2, INPUT_PULLUP);
    pinMode(btn3, INPUT_PULLUP);
    pinMode(btn4, INPUT_PULLUP);
    pinMode(Func1, OUTPUT);
    pinMode(Func2, OUTPUT);
    pinMode(Func3, OUTPUT);
    randomSeed(analogRead(1));
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_11_mf);
    u8g2.drawStr(15, 40, ("Welcome to ..."));
    u8g2.setFont(u8g2_font_crox5hb_tf);
    u8g2.drawStr(5, 30, "Watch 10!");
    u8g2.sendBuffer();
    for(int i=0; i<100; i++){
      delay(40);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      if (button_is_pressed(btn3)){
        digitalWrite(Func1, HIGH);  
      }
      else if (button_is_pressed(btn2)){
        digitalWrite(Func3, HIGH);  
      }
    }
}


bool button_is_pressed(int btn)
{
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
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_tinyunicode_tf);
    u8g2.drawStr(0, 15, "1. Quick Flash");
    u8g2.drawStr(0, 20, "2. Always On");
    u8g2.drawStr(0, 25, "3. Blink");
    u8g2.drawStr(0, 30, "4. Return");
    
    u8g2.setCursor(10, 40);
    u8g2.print("State: ");
    u8g2.print(digitalRead(func));
    
    u8g2.sendBuffer();

    if (blink){
      digitalWrite(func, !digitalRead(func));
      delay(blinkTime);
    }

    if (button_is_pressed(btn1))
      digitalWrite(func, HIGH);
    else
      digitalWrite(func, LOW);

    if (button_is_pressed(btn2)){
      if (digitalRead(func) == HIGH)
        digitalWrite(func, HIGH);
      else 
        digitalWrite(func, LOW);
      delay(500);
    }
    else if (button_is_pressed(btn3))
      blink = !blink;

    else if (button_is_pressed(btn4)){
      return;
    }
  }
}

void watchFuncs(void)
{
  delay(50);
  while(true){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_tinyunicode_tf);
    u8g2.drawStr(0, 15, "1. White LED");
    u8g2.drawStr(0, 20, "2. Fan");
    u8g2.drawStr(0, 25, "3. Laser");
    u8g2.drawStr(0, 30, "4. UV LED");
    
    u8g2.setCursor(10, 40);
    u8g2.print("Sel: ");
    u8g2.print(selectedPart);
    
    u8g2.sendBuffer();
    
    if (button_is_pressed(btn2))
    {
      selectedPart++;
      if (selectedPart > totalParts) selectedPart = 1;
    }

    else if (button_is_pressed(btn1))
    {
      selectedPart--;
      if (selectedPart < 1) selectedPart = totalParts;
    }

    else if (button_is_pressed(btn4)) return;

    else if (button_is_pressed(btn3))
    {
      switch (selectedPart)
      {
        case 1:
          activateFunc(Func1);
          continue;
        case 2:
          activateFunc(Func2, 1000);
          continue;
        case 3:
          activateFunc(Func3);
          continue;
        case 4:
          activateFunc(Func4, 200);
          continue;
      }
    }

    delay(50);
  }
}

void calculator(void) {
    const char* options[] = {"1","2","3","4","5","6","7","8","9","0",".","+","-","*","/","^","√","%","(",")"};
    const int numOptions = sizeof(options)/sizeof(options[0]);
    int currentOption = 0;
    char expr[25] = "";
    int exprLen = 0;
    bool done = false;
    bool showResult = false;
    double result = 0;
    bool error = false;

    while (!done) {
        u8g2.clearBuffer();

        // Draw current expression
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 40, expr);

        // Display "Sel:" label
        u8g2.drawStr(0, 10, "Sel:");

        // Draw options with highlight on current selection
        int startX = 30; // starting x position for options
        int yPos = 10;   // y position for options
        int spacing = 20; // space between options

        for (int i = 0; i < numOptions; i++) {
            int x = startX + i * spacing;
            if (i == currentOption) {
                // Highlight selected option
                u8g2.setDrawColor(0); // Set background to black (inverted)
                u8g2.drawBox(x - 1, yPos - 8, spacing - 2, 10); // draw background box
                u8g2.setDrawColor(1); // Set back to normal color
                u8g2.setFont(u8g2_font_ncenB08_tr);
                u8g2.drawStr(x, yPos, options[i]);
            } else {
                // Normal display
                u8g2.setFont(u8g2_font_ncenB08_tr);
                u8g2.drawStr(x, yPos, options[i]);
            }
        }

        // Show result if needed
        if (showResult) {
            if (error) {
                u8g2.drawStr(10, 20, "Error!");
            } else {
                u8g2.setCursor(0, 64);
                u8g2.print("= ");
                u8g2.print(result, 6);
            }
        }

        u8g2.sendBuffer();

        // Button handling
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
            done = true;
            delay(200);
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
    if (*s == '√') { s++; double val = parseFactor(s, error); return val<0?(error=true,0):sqrt(val); }
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
    unsigned long lastActionTime = 0;
    const unsigned long debounceDelay = 100;
    
    while (inRandomInt) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_t0_11_mf);
        u8g2.drawStr(10, 20, "Random Int Mode");
        u8g2.setCursor(10, 40);
        u8g2.print("Range: 0 - ");
        u8g2.print(range);
        u8g2.sendBuffer();

        if (button_is_pressed(btn1)) {
            range += 10;
            delay(200);
        } 
        else if (button_is_pressed(btn2)) {
            range--;
            delay(200);
        } 
        else if (button_is_pressed(btn3)) {
            int randNumber = random(0, range + 1);
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_t0_18b_tf);
            u8g2.setCursor(10, 40);
            u8g2.print("Rnd: ");
            u8g2.print(randNumber);
            u8g2.sendBuffer();
            delay(3000);
        } 
        else if (button_is_pressed(btn4)) {
            inRandomInt = false;
        }
        delay(50);
    }
}

void loop()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_boutique_bitmap_9x9_te);
  u8g2.drawStr(0, 18, "1. Outputs 2. Maths");
  u8g2.drawStr(0, 28, "3. Random 4. Blink");
  u8g2.setFont(u8g2_font_tinyunicode_tr);
  u8g2.setCursor(10, 38);
  u8g2.print("Option ");
  u8g2.print(selectedFunction);
  u8g2.print(" Selected");
  u8g2.sendBuffer();

  if (button_is_pressed(btn2))
  {
    selectedFunction++;
    if (selectedFunction > totalFunctions) selectedFunction = 1;
  }

  else if (button_is_pressed(btn1))
  {
    selectedFunction--;
    if (selectedFunction < 1){
      selectedFunction = totalFunctions;
    }
  }

  else if (button_is_pressed(btn3))
  {
    switch (selectedFunction)
    {
      case 1:
        watchFuncs(); break;
      case 2:
        calculator(); break;
      case 3:
        randomInt(); break;
      case 4:
        for (int i = 1000; i <= 1; i--){
          digitalWrite(LED_BUILTIN, HIGH);
          delay(1000);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);
        }
    }
  }

  delay(20);
}
