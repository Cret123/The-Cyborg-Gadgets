// Main Watch program for arduino Nano watch extension
// fonts https://github.com/olikraus/u8g2/wiki/fntlistall#4-pixel-height

#include <U8g2lib.h>

U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


const byte BtnEnter = 2;
const byte Btn1 = 4;
const byte Btn2 = 7;
const byte Btn3 = 9;

const byte Func1 = 3;
const byte Func2 = 6;
const byte Func3 = 8;


void setup()
{
    pinMode(Btn1, INPUT);
    pinMode(Btn2, INPUT);
    pinMode(Btn3, INPUT);
    pinMode(BtnEnter, INPUT);
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_11_mf);
    u8g2.drawStr(40, 40, "Hello!");
    u8g2.drawStr(12, 20, "Welcome to Watch");
    u8g2.setFont(u8g2_font_t0_18b_tf );
    u8g2.drawStr(50, 32, "10!");
    u8g2.sendBuffer();
    delay(5000);
}

void loop()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tinyunicode_tf);
  u8g2.drawStr(0, 40, "Options:");
  u8g2.drawStr(0, 15, "- Fan");
  u8g2.drawStr(0, 20, "- Calculator");
  u8g2.drawStr(0, 25, "- LEDs");
  u8g2.drawStr(0, 30, "- Laser");
  u8g2.sendBuffer();
  if (digitalRead(Btn2) == LOW){
     digitalWrite(LED_BUILTIN, LOW);
     delay(10000);
  }  
}
