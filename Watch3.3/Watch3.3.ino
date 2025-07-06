// Real time watch proram

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <RTClib.h> // (DS3231/DS1307)

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

const byte btn1 = 6;
const byte btn2 = 4;
const byte btn3 = 3;
const byte btn4 = 5;
const byte btn5 = 7;

enum WatchFunction {
  FUNC_CLOCK,
  FUNC_TIMER,
  FUNC_STOPWATCH,
  FUNC_ALARM,
  FUNC_CALCULATOR,
  FUNC_LED1,
  FUNC_LED2,
  FUNC_LED3,
  FUNC_LED4,
  FUNC_BUILTINLED,
  NUM_FUNCTIONS
};

int currentFunction = FUNC_CLOCK;

const byte Func1 = 8;
const byte Func2 = 9;
const byte Func3 = 10;
const byte Func4 = 11;

struct FuncState {
  bool blink = false;
  bool keepOn = false;
  int blinkTime = 500;
  unsigned long lastBlink = 0;
  bool outputState = false;
};

FuncState funcStates[NUM_FUNCTIONS];

bool button_is_pressed(byte btn) {
  if (digitalRead(btn) == LOW) {
    delay(100);
    return true;
  }
  return false;
}

void showClock() {
  static bool setMode = false;
  static int setField = 0;
  static DateTime setTime;
  static unsigned long lastBlink = 0;
  static bool blinkOn = true;

  DateTime now = rtc.now();
  if (!setMode) setTime = now;

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  int hour = setMode ? setTime.hour() : now.hour();
  int min = setMode ? setTime.minute() : now.minute();
  int sec = setMode ? setTime.second() : now.second();
  int day = setMode ? setTime.day() : now.day();
  int month = setMode ? setTime.month() : now.month();
  int year = setMode ? setTime.year() : now.year();

  char buf[9];
  if (setMode && setField == 0 && !blinkOn)
    sprintf(buf, "  :%02d:%02d", min, sec);
  else if (setMode && setField == 1 && !blinkOn)
    sprintf(buf, "%02d:  :%02d", hour, sec);
  else if (setMode && setField == 2 && !blinkOn)
    sprintf(buf, "%02d:%02d:  ", hour, min);
  else
    sprintf(buf, "%02d:%02d:%02d", hour, min, sec);
  display.print(buf);

  display.setTextSize(1);
  display.setCursor(0, 28);

  if (setMode && setField == 3 && !blinkOn)
    sprintf(buf, "  /%02d/%04d", month, year);
  else if (setMode && setField == 4 && !blinkOn)
    sprintf(buf, "%02d/  /%04d", day, year);
  else if (setMode && setField == 5 && !blinkOn)
    sprintf(buf, "%02d/%02d/    ", day, month);
  else
    sprintf(buf, "%02d/%02d/%04d", day, month, year);
  display.print(buf);

  display.setCursor(0, 55);
  if (!setMode) {
    display.print("Btn1: Set Btn4/5: Switch");
  } else {
    const char* fields[] = {"Hour","Min","Sec","Day","Month","Year"};
    display.print(fields[setField]);
    display.print(" Btn1: Next Btn2/3: +/-");
  }

  display.display();

  if (!setMode && button_is_pressed(btn1)) {
    setMode = true;
    setField = 0;
    setTime = now;
    delay(200);
    return;
  }

  if (setMode) {
    if (millis() - lastBlink > 400) {
      blinkOn = !blinkOn;
      lastBlink = millis();
    }
    if (button_is_pressed(btn1)) {
      setField++;
      if (setField >= 6) {
        rtc.adjust(DateTime(setTime.year(), setTime.month(), setTime.day(),
                            setTime.hour(), setTime.minute(), setTime.second()));
        setMode = false;
      }
      delay(250);
      return;
    }
    if (button_is_pressed(btn2)) {
      switch (setField) {
        case 0: setTime = DateTime(setTime.year(), setTime.month(), setTime.day(), (setTime.hour() + 1) % 24, setTime.minute(), setTime.second()); break;
        case 1: setTime = DateTime(setTime.year(), setTime.month(), setTime.day(), setTime.hour(), (setTime.minute() + 1) % 60, setTime.second()); break;
        case 2: setTime = DateTime(setTime.year(), setTime.month(), setTime.day(), setTime.hour(), setTime.minute(), (setTime.second() + 1) % 60); break;
        case 3: setTime = DateTime(setTime.year(), setTime.month(), (setTime.day() % 31) + 1, setTime.hour(), setTime.minute(), setTime.second()); break;
        case 4: setTime = DateTime(setTime.year(), (setTime.month() % 12) + 1, setTime.day(), setTime.hour(), setTime.minute(), setTime.second()); break;
        case 5: setTime = DateTime(setTime.year() + 1, setTime.month(), setTime.day(), setTime.hour(), setTime.minute(), setTime.second()); break;
      }
      delay(150);
      return;
    }
    if (button_is_pressed(btn3)) {
      switch (setField) {
        case 0: setTime = DateTime(setTime.year(), setTime.month(), setTime.day(), (setTime.hour() + 23) % 24, setTime.minute(), setTime.second()); break;
        case 1: setTime = DateTime(setTime.year(), setTime.month(), setTime.day(), setTime.hour(), (setTime.minute() + 59) % 60, setTime.second()); break;
        case 2: setTime = DateTime(setTime.year(), setTime.month(), setTime.day(), setTime.hour(), setTime.minute(), (setTime.second() + 59) % 60); break;
        case 3: setTime = DateTime(setTime.year(), setTime.month(), (setTime.day() + 29) % 31 + 1, setTime.hour(), setTime.minute(), setTime.second()); break;
        case 4: setTime = DateTime(setTime.year(), (setTime.month() + 10) % 12 + 1, setTime.day(), setTime.hour(), setTime.minute(), setTime.second()); break;
        case 5: setTime = DateTime(setTime.year() - 1, setTime.month(), setTime.day(), setTime.hour(), setTime.minute(), setTime.second()); break;
      }
      delay(150);
      return;
    }
  }
}

unsigned long timerSet = 60;
unsigned long timerLeft = 0;
bool timerRunning = false;
unsigned long timerLast = 0;

void showTimer() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  unsigned long t = timerRunning ? timerLeft : timerSet;
  byte m = t / 60;
  byte s = t % 60;
  char buf[6];
  sprintf(buf, "%02d:%02d", m, s);
  display.print("TMR ");
  display.print(buf);
  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print(timerRunning ? "Running" : "Set: Btn2/Btn3 +/-");
  display.setCursor(0, 40);
  display.print("Btn1: Start/Stop");

  display.display();

  if (!timerRunning) {
    if (button_is_pressed(btn2) && timerSet < 3600) timerSet += 10;
    if (button_is_pressed(btn3) && timerSet >= 10) timerSet -= 10;
    if (button_is_pressed(btn1)) {
      timerLeft = timerSet;
      timerRunning = true;
      timerLast = millis();
    }
  } else {
    if (millis() - timerLast >= 1000 && timerLeft > 0) {
      timerLeft--;
      timerLast += 1000;
    }
    if (timerLeft == 0) timerRunning = false;
    if (button_is_pressed(btn1)) timerRunning = false;
  }
}

unsigned long stopwatchTime = 0;
bool stopwatchRunning = false;
unsigned long stopwatchLast = 0;

void showStopwatch() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  unsigned long t = stopwatchTime;
  if (stopwatchRunning) {
    unsigned long now = millis();
    t += now - stopwatchLast;
  }
  byte m = (t / 1000) / 60;
  byte s = (t / 1000) % 60;
  char buf[9];
  sprintf(buf, "%02d:%02d.%01d", m, s, (int)((t % 1000) / 100));
  display.print("SW ");
  display.print(buf);

  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print("Btn1: Start/Stop  Btn2: Reset");

  display.display();

  if (button_is_pressed(btn1)) {
    if (!stopwatchRunning) {
      stopwatchLast = millis();
      stopwatchRunning = true;
    } else {
      stopwatchTime += millis() - stopwatchLast;
      stopwatchRunning = false;
    }
    delay(200);
  }
  if (button_is_pressed(btn2)) {
    stopwatchTime = 0;
    if (stopwatchRunning) stopwatchLast = millis();
    delay(200);
  }
  if (!stopwatchRunning && stopwatchTime > 0) stopwatchLast = millis();
}

byte alarmHour = 7, alarmMin = 0;
bool alarmSet = false, alarmActive = false;

void showAlarm() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  char buf[6];
  sprintf(buf, "%02d:%02d", alarmHour, alarmMin);
  display.print("AL ");
  display.print(buf);

  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print(alarmSet ? "Set" : "OFF");
  display.setCursor(0, 35);
  display.print("Btn1: Set ON/OFF");
  display.setCursor(0, 45);
  display.print("Btn2/3: Hour/Min");

  display.display();

  if (button_is_pressed(btn1)) {
    alarmSet = !alarmSet;
    delay(200);
  }
  if (button_is_pressed(btn2)) {
    alarmHour = (alarmHour + 1) % 24;
    delay(100);
  }
  if (button_is_pressed(btn3)) {
    alarmMin = (alarmMin + 1) % 60;
    delay(100);
  }

  DateTime now = rtc.now();
  if (alarmSet && now.hour() == alarmHour && now.minute() == alarmMin && now.second() == 0 && !alarmActive) {
    alarmActive = true;
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_BUILTIN, HIGH); delay(100);
      digitalWrite(LED_BUILTIN, LOW); delay(100);
    }
  }
  if (now.minute() != alarmMin) alarmActive = false;
}

void showCalculator() {
  static char expr[25] = "";
  static int exprLen = 0;
  static bool showResult = false;
  static double result = 0;
  static bool error = false;
  const char* options[] = {"1","2","3","4","5","6","7","8","9","0",".","+","-","*","/","(",")"};
  const int numOptions = sizeof(options)/sizeof(options[0]);
  static int currentOption = 0;

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 0);
  if (showResult) {
    if (error) display.print("Error!");
    else display.print(result, 6);
  }

  display.setTextSize(1);
  int startX = 0;
  int yOptions = 20;
  int spacing = 16;
  for (int i = 0; i < numOptions; i++) {
    int x = startX + (i % 7) * spacing;
    int y = yOptions + (i / 7) * 10;
    if (i == currentOption) {
      display.fillRect(x-1, y-1, 12, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(x, y);
      display.print(options[i]);
      display.setTextColor(SSD1306_WHITE);
    } else {
      display.setCursor(x, y);
      display.print(options[i]);
    }
  }
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print(expr);

  display.display();

  if (button_is_pressed(btn2)) {
    currentOption = (currentOption + 1) % numOptions;
    delay(100);
  } else if (button_is_pressed(btn3)) {
    if (showResult) {
      expr[0] = '\0';
      exprLen = 0;
      showResult = false;
    }
    if (exprLen < 24) {
      strcat(expr, options[currentOption]);
      exprLen = strlen(expr);
    }
    delay(200);
  } else if (button_is_pressed(btn1)) {
    error = false;
    result = evaluateExpression(expr, error);
    showResult = true;
    delay(500);
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
  if (*s == '(') {
    s++;
    double val = parsePrimary(s, error);
    if (*s == ')') s++;
    else error = true;
    return val;
  }
  return parseNumber(s, error);
}

double parseTerm(const char* &s, bool &error) {
  double left = parseFactor(s, error);
  while (*s == '*' || *s == '/') {
    char op = *s++;
    double right = parseFactor(s, error);
    if (op == '*') left *= right;
    else if (op == '/') {
      if (right == 0) { error = true; return 0; }
      left /= right;
    }
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

void showFunc(const char* name, const byte func, int idx) {
  FuncState &state = funcStates[idx];
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("1. Quick Flash");
  display.setCursor(0, 30);
  display.println("2. Always On");
  display.setCursor(0, 40);
  display.println("3. Blink");
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.print(name);
  display.display();

  if (state.blink) {
    unsigned long now = millis();
    if (now - state.lastBlink >= state.blinkTime) {
      state.outputState = !state.outputState;
      digitalWrite(func, state.outputState ? HIGH : LOW);
      state.lastBlink = now;
    }
  } else if (state.keepOn) {
    digitalWrite(func, HIGH);
  } else {
    if (digitalRead(btn1) == LOW) digitalWrite(func, HIGH);
    else digitalWrite(func, LOW);
  }

  if (button_is_pressed(btn2)) {
    state.keepOn = !state.keepOn;
    if (state.keepOn) state.blink = false;
    delay(200);
  } else if (button_is_pressed(btn3)) {
    state.blink = !state.blink;
    if (state.blink) {
      state.keepOn = false;
      state.lastBlink = millis();
      state.outputState = false;
      digitalWrite(func, LOW);
    }
    delay(200);
  }
}

void setup() {
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btn5, INPUT_PULLUP);
  pinMode(Func1, OUTPUT);
  pinMode(Func2, OUTPUT);
  pinMode(Func3, OUTPUT);
  pinMode(Func4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!rtc.begin()) {
  }
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    while (true){
      digitalWrite(LED_BUILTIN, HIGH); delay(200);
      digitalWrite(LED_BUILTIN, LOW); delay(200);
    }
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(7, 0);
  display.print("Wecome to");
  display.setCursor(5, 35);
  display.print("Watch 3.3!");
  display.display();
  delay(1200);
  currentFunction = FUNC_CLOCK;
}

void loop() {
  if (button_is_pressed(btn4)) {
    currentFunction--;
    if (currentFunction < 0) currentFunction = NUM_FUNCTIONS - 1;
    delay(200);
  }
  if (button_is_pressed(btn5)) {
    currentFunction++;
    if (currentFunction >= NUM_FUNCTIONS) currentFunction = 0;
    delay(200);
  }

  switch (currentFunction) {
    case FUNC_CLOCK:
      showClock();
      break;
    case FUNC_TIMER:
      showTimer();
      break;
    case FUNC_STOPWATCH:
      showStopwatch();
      break;
    case FUNC_ALARM:
      showAlarm();
      break;
    case FUNC_CALCULATOR:
      showCalculator();
      break;
    case FUNC_LED1:
      showFunc("White LED", Func1, FUNC_LED1);
      break;
    case FUNC_LED2:
      showFunc("Laser", Func2, FUNC_LED2);
      break;
    case FUNC_LED3:
      showFunc("UV LED", Func3, FUNC_LED3);
      break;
    case FUNC_LED4:
      showFunc("LED4", Func4, FUNC_LED4);
      break;
    case FUNC_BUILTINLED:
      showFunc("BuiltIn", LED_BUILTIN, FUNC_BUILTINLED);
      break;
  }
  delay(50);
}
