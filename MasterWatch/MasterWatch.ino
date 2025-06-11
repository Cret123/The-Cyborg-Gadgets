#include <U8g2lib.h>
#include <avr/sleep.h>

U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

bool button_is_pressed(int btn);

int selectedFunction = 1;
const int totalFunctions = 4;

const byte btn1 = 2;
const byte btn2 = 3;
const byte btn3 = 4;
const byte btn4 = 5;

const byte Func1 = 2;
const byte Func2 = 3;
const byte Func3 = 4;

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
    digitalWrite(LED_BUILTIN, LOW);
}

void randomInt() {
    int range = 10;  // Starting range
    bool inRandomInt = true;
    unsigned long lastActionTime = 0;
    const unsigned long debounceDelay = 100; // Optional, can use your global debounce

    while (inRandomInt) {
        // Display the current range
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_t0_11_mf);
        u8g2.drawStr(10, 20, "Random Int Mode");
        u8g2.setCursor(10, 40);
        u8g2.print("Range: 0 - ");
        u8g2.print(range);
        u8g2.sendBuffer();

        if (button_is_pressed(btn1)) {
            range += 1;
            delay(200); // Simple debounce
        } else if (button_is_pressed(btn2)) {
            range += 10;
            delay(200);
        } else if (button_is_pressed(btn3)) {
            int randNumber = random(0, range + 1);
            // Display the random number
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_t0_18b_tf);
            u8g2.setCursor(10, 40);
            u8g2.print("Rnd: ");
            u8g2.print(randNumber);
            u8g2.sendBuffer();
            delay(1500); // Show the number for a moment
        } else if (button_is_pressed(btn4)) {
            inRandomInt = false; // Exit to loop
        }
        delay(50); // Loop delay to reduce CPU usage
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
        //sleep_mode();
        digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  delay(50);
}
}
