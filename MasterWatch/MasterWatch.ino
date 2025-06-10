#include <U8g2lib.h>
#include <avr/sleep.h>

U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int buttonPins = A0;

int selectedFunction = 1;
const int totalFunctions = 4;

const int threshold1 = 600;
const int threshold2 = 400;
const int threshold3 = 200;
const int threshold4 = 10;

const byte Func1 = 2;
const byte Func2 = 3;
const byte Func3 = 4;

unsigned long lastButtonCheck = 0;
const unsigned long debounceDelay = 100;

bool lastThreshold1State = false;
bool lastThreshold2State = false;
bool lastThreshold4State = false;
bool lastThreshold3State = false;

bool button_is_pressed(int threshold);

int timer = 5000;

void setup()
{
    pinMode(buttonPins, INPUT);
    pinMode(Func1, OUTPUT);
    pinMode(Func2, OUTPUT);
    pinMode(Func3, OUTPUT);
    randomSeed(analogRead(1));
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    u8g2.begin();
    u8g2.sendBuffer();
    for (int i = timer; i > 0; i--){
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_t0_11_mf);
      u8g2.drawStr(40, 40, i);
      u8g2.drawStr(12, 20, "Welcome to Watch");
      u8g2.setFont(u8g2_font_t0_18b_tf );
      u8g2.drawStr(50, 32, "10!");
      u8g2.sendBuffer();
      delay(1);
      if (button_is_pressed(threshold4)){
        return;
        }
    }
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_11_mf);
    u8g2.drawStr(12, 20, "Calling The Police,");
    u8g2.drawStr(12, 30, "Thief!");
    u8g2.sendBuffer();
    delay(5000);
    sleep_mode();
}


bool button_is_pressed(int threshold)
{
    unsigned long currentTime = millis();
    static bool lastState = false;
    bool currentVal = analogRead(buttonPins);
    bool currentThresholdActive = currentVal >= threshold;

    bool pressed = false;

    if (currentThresholdActive && !lastState && (currentTime - lastButtonCheck > debounceDelay))
    {
        pressed = true;
        lastButtonCheck = currentTime;
    }

    lastState = currentThresholdActive;
    return pressed;
}

void watchFuncs(void)
{
    digitalWrite(LED_BUILTIN, LOW);
}

void randomInt() {
    static int clickCount = 10;
    static int prevBtn3State = HIGH;
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50;
    int range = clickCount;
    int randNumber = random(0, range + 1);
    for (int i = 0; i < randNumber; i++){
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
    }
}

void loop()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tinyunicode_tf);
  u8g2.drawStr(0, 15, "1. Watch Funcs");
  u8g2.drawStr(0, 20, "2. Calculator");
  u8g2.drawStr(0, 25, "3. Random Int");
  u8g2.drawStr(0, 30, "4. Turn Off");
  
  u8g2.setCursor(10, 40);
  u8g2.print("Sel: ");
  u8g2.print(selectedFunction);
  
  u8g2.sendBuffer();

  unsigned long currentTime = millis();

  if (button_is_pressed(threshold1))
  {
    selectedFunction++;
    if (selectedFunction > totalFunctions) selectedFunction = 1;
  }

  else if (button_is_pressed(threshold2))
  {
    selectedFunction--;
    if (selectedFunction < 1){
      selectedFunction = totalFunctions;
  }

  else if (button_is_pressed(threshold4))
  {
    switch (selectedFunction)
    {
      case 1:
        watchFuncs();
        break;
      case 2:
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        break;
      case 3:
        randomInt;
        break;
      case 4:
        sleep_mode();
    }
  }

  delay(50);
}
