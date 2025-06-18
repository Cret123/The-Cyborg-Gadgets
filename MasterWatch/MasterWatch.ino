#include <U8g2lib.h>
#include <avr/sleep.h>
#include <EEPROM.h>

U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

bool button_is_pressed(int btn);

int selectedFunction = 1;
const int totalFunctions = 4;

int selectedPart = 1;
const int totalParts = 4;

const byte btn1 = 6;
const byte btn2 = 3;
const byte btn3 = 4;
const byte btn4 = 5;

const byte Func1 = 7;
const byte Func2 = 8;
const byte Func3 = 9;
const byte Func4 = 10;

int timer = 5000;

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
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_11_mf);
    u8g2.drawStr(12, 20, ("Welcome to Watch"));
    u8g2.setFont(u8g2_font_t0_18b_tf );
    u8g2.drawStr(50, 32, "10!");
    u8g2.sendBuffer();
    delay(timer);

}


bool button_is_pressed(int btn)
{
  if (digitalRead(btn) == LOW){
    delay(50);
    return true;
    }
  return false;
}

void watchFuncs(void)
{
  while(true){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_tinyunicode_tf);
    u8g2.drawStr(0, 15, "1. White LED");
    u8g2.drawStr(0, 20, "2. Fan");
    u8g2.drawStr(0, 25, "3. UV LED");
    u8g2.drawStr(0, 30, "4. Laser");
    
    u8g2.setCursor(10, 40);
    u8g2.print("Sel: ");
    u8g2.print(selectedPart);
    
    u8g2.sendBuffer();

    unsigned long currentTime = millis();

    if (button_is_pressed(btn1))
    {
      selectedPart++;
      if (selectedPart > totalParts) selectedPart = 1;
    }

    else if (button_is_pressed(btn2))
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
          while (true){
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_tinyunicode_tf);
            u8g2.drawStr(0, 15, "1. Quick Flash");
            u8g2.drawStr(0, 20, "2. Always On");
            u8g2.drawStr(0, 25, "3. -");
            u8g2.drawStr(0, 30, "4. Return");
            
            u8g2.setCursor(10, 40);
            u8g2.print("State: ");
            u8g2.print(digitalRead(Func1));
            
            u8g2.sendBuffer();

            if (button_is_pressed(btn1))
              digitalWrite(Func1, HIGH);
            else
              digitalWrite(Func1, LOW);

            if (button_is_pressed(btn2))
              digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

            else if (button_is_pressed(btn4)){
              break;
            }
          }
            
        case 2:
          while (true){
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_tinyunicode_tf);
            u8g2.drawStr(0, 15, "1. Quick Flash");
            u8g2.drawStr(0, 20, "2. Always On");
            u8g2.drawStr(0, 25, "3. -");
            u8g2.drawStr(0, 30, "4. Return");
            
            u8g2.setCursor(10, 40);
            u8g2.print("State: ");
            u8g2.print(digitalRead(Func2));
            
            u8g2.sendBuffer();

            if (button_is_pressed(btn1))
              digitalWrite(Func2, HIGH);
            else
              digitalWrite(Func2, LOW);

            if (button_is_pressed(btn2))
              digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

            else if (button_is_pressed(btn4)){
              break;
            }
          }
        case 3:
          while (true){
            u8g2.clearBuffer();
            u8g2.drawStr(0, 15, "1. Quick Flash");
            u8g2.drawStr(0, 20, "2. Always On");
            u8g2.drawStr(0, 25, "3. -");
            u8g2.drawStr(0, 30, "4. Return");
            
            u8g2.setCursor(10, 40);
            u8g2.print("State: ");
            u8g2.print(digitalRead(Func3));
            
            u8g2.sendBuffer();

            if (button_is_pressed(btn1))
              digitalWrite(Func3, HIGH);
            else
              digitalWrite(Func3, LOW);

            if (button_is_pressed(btn2))
              digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

            else if (button_is_pressed(btn4)){
              break;
            }
          }
        case 4:
          while (true){
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_tinyunicode_tf);
            u8g2.drawStr(0, 15, "1. Quick Flash");
            u8g2.drawStr(0, 20, "2. Always On");
            u8g2.drawStr(0, 25, "3. -");
            u8g2.drawStr(0, 30, "4. Return");
            
            u8g2.setCursor(10, 40);
            u8g2.print("State: ");
            u8g2.print(digitalRead(Func4));
            
            u8g2.sendBuffer();

            if (button_is_pressed(btn1))
              digitalWrite(Func4, HIGH);
            else
              digitalWrite(Func4, LOW);

            if (button_is_pressed(btn2))
              digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

            else if (button_is_pressed(btn4)){
              break;
            }
          }
      }
    }

    delay(50);
  }
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
            range ++;
            delay(200);
        } else if (button_is_pressed(btn2)) {
            range += 10;
            delay(200);
        } else if (button_is_pressed(btn3)) {
            int randNumber = random(0, range + 1);
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_t0_18b_tf);
            u8g2.setCursor(10, 40);
            u8g2.print("Rnd: ");
            u8g2.print(randNumber);
            u8g2.sendBuffer();
            delay(3000);
        } else if (button_is_pressed(btn4)) {
            inRandomInt = false;
        }
        delay(50);
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

  if (button_is_pressed(btn1))
  {
    selectedFunction++;
    if (selectedFunction > totalFunctions) selectedFunction = 1;
  }

  else if (button_is_pressed(btn2))
  {
    selectedFunction--;
    if (selectedFunction < 1){
      selectedFunction = totalFunctions;
    }
  }

  else if (button_is_pressed(btn4))
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
