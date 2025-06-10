#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
}

// fonts https://github.com/olikraus/u8g2/wiki/fntlistall#4-pixel-height

void loop()
{
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_u8glib_4_tf);  // choose a suitable font
  u8g2.drawStr(0, 5, "Hello,");  // write something to the internal memory
  u8g2.drawStr(0, 10, "World…");
  u8g2.drawStr(0, 15, "I'm tiny…");
  u8g2.drawStr(0, 20, "So tiny!");
  u8g2.drawStr(0, 25, "However you can");
  u8g2.drawStr(0, 30, "have six lines");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_t0_11_tf);  // choose a suitable font
  u8g2.drawStr(0, 10, "Hello,");  // write something to the internal memory
  u8g2.drawStr(0, 20, "World…");
  u8g2.drawStr(0, 30, "I'm tiny…");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_tenstamps_mf);  // choose a suitable font
  u8g2.drawStr(0, 12, "ABCD");  // write something to the internal memory
  u8g2.drawStr(0, 30, "1234");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);

  for (int a = 999; a >= 0; --a)
  {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_inb24_mr );  // choose a suitable font
    u8g2.setCursor(0, 24);
    u8g2.print(a);
    u8g2.sendBuffer();          // transfer internal memory to the display
    delay(5);
  }
  delay(1000);
}
