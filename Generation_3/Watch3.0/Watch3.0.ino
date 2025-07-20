// Watch 3.0: Initial Generation 3 watch, communicates via neopixel

#include <Adafruit_NeoPixel.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

int Wheel(byte WheelPos);

#define NEOPIXEL_PIN 9
#define NEOPIXEL_COUNT 1

Adafruit_NeoPixel pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const byte btn1 = 2;
const byte btn2 = 3;
const byte btn3 = 4;
const byte btn4 = 5;

const byte Func1 = 6;
const byte Func2 = 7;
const byte Func3 = 8;
const byte Func4 = 9;

int selectedFunction = 1;
const int totalFunctions = 5;

volatile bool wakeup = false;

void setup(){
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(Func1, OUTPUT);
  pinMode(Func2, OUTPUT);
  pinMode(Func3, OUTPUT);
  pinMode(Func4, OUTPUT);
  pixel.begin();
  pixel.clear();
  for (int i = 0; i <= 255; i++) {
    pixel.setPixelColor(0, Wheel(i));
    pixel.show();
    delay(5);
  }
  
  pixel.clear();
  pixel.show();
  delay(200);
  indicateFunction(selectedFunction);
}

bool button_is_pressed(int btn){
  if (digitalRead(btn) == LOW){
    delay(100);
    return true;
  }
  return false;
}

void(* reset) (void) = 0;

void wakeUp(){
    wakeup = true;
}

void goToSleep(){
    pixel.clear();
    pixel.show();

    sleep_bod_disable();
    wakeup = false;
    attachInterrupt(digitalPinToInterrupt(btn3), wakeUp, FALLING);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();

    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(btn3));
}

void blinkColor(uint32_t color, int times, int on_ms = 150, int off_ms = 180){
  for (int i = 0; i < times; i++){
    pixel.setPixelColor(0, color);
    pixel.show();
    delay(on_ms);
    pixel.clear();
    pixel.show();
    delay(off_ms);
  }
}

int Wheel(int WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void indicateFunction(int funcNum){
  uint32_t colors[] ={
    pixel.Color(255, 255, 255), // 1: Output Control
    pixel.Color(255, 0, 0),     // 2: Color Picker
    pixel.Color(0, 255, 0),     // 3: Bomb/Detonate
    pixel.Color(0, 0, 255),     // 4: Sequence Game
    pixel.Color(255, 255, 0)    // 5: Sleep Mode
  };
  pixel.clear();
  pixel.setPixelColor(0, colors[funcNum - 1]);
  pixel.show();
}

void outputs(){
  int selected = 1;
  const int total = 4;
  const uint32_t outColors[] = {
    pixel.Color(255,255,255), // Func1: White
    pixel.Color(255,0,0),     // Func2: Red
    pixel.Color(144,99,205),  // Func3: Violet
    pixel.Color(0,255,0)      // Func4: Green
  };
  const uint8_t outPins[] = {Func1, Func2, Func3, Func4};

  while (true){
    blinkColor(outColors[selected-1], 1, 200, 300);

    if (button_is_pressed(btn2)){
      selected++; if (selected > total) selected = 1;
      delay(150);
    } else if (button_is_pressed(btn1)){
      selected--; if (selected < 1) selected = total;
      delay(150);
    } else if (button_is_pressed(btn3)){
      uint8_t pin = outPins[selected-1];
      bool isOn = digitalRead(pin);
      digitalWrite(pin, !isOn);
      blinkColor(outColors[selected-1], !isOn ? 2 : 1, 250, 100);
      delay(200);
    } else if (button_is_pressed(btn4)){
      for (int i=0;i<total;++i) digitalWrite(outPins[i], LOW);
      pixel.clear(); pixel.show();
      delay(200);
      return;
    }
    delay(50);
  }
}

void colorPicker(){
  int r = 128, g = 0, b = 0;
  int sel = 0;
  while (true){
    pixel.setPixelColor(0, pixel.Color(r, g, b));
    pixel.show();
    delay(30);

    if (button_is_pressed(btn1)){ sel = (sel + 1) % 3; delay(160);}
    else if (button_is_pressed(btn2)){ sel = (sel + 2) % 3; delay(160);}
    else if (button_is_pressed(btn3)){
      if (sel == 0) r = (r + 32) % 256;
      else if (sel == 1) g = (g + 32) % 256;
      else b = (b + 32) % 256;
      delay(130);
    }
    else if (button_is_pressed(btn4)){
      blinkColor(pixel.Color(r,g,b), 2, 250, 100);
      pixel.setPixelColor(0, pixel.Color(r,g,b));
      pixel.show();
      delay(1500);
      pixel.clear(); pixel.show();
      return;
    }
  }
}

void randomInt() {
  int maxVal = 9999;
  while (true) {
    blinkColor(pixel.Color(255,255,255), 2, 80, 80);

    bool changed = false;
    unsigned long t0 = millis();
    while (millis() - t0 < 1800) {
      if (button_is_pressed(btn1)) { maxVal = (maxVal < 9999) ? maxVal + 1 : 1; changed = true; t0 = millis(); }
      else if (button_is_pressed(btn2)) { maxVal = (maxVal > 1) ? maxVal - 1 : 9999; changed = true; t0 = millis(); }
      else if (button_is_pressed(btn4)) { return; }
      else if (button_is_pressed(btn3)) { break; }
      delay(10);
    }
    if (changed) {
      int d[4];
      int tmp = maxVal;
      d[0] = tmp/1000; tmp %= 1000;
      d[1] = tmp/100;  tmp %= 100;
      d[2] = tmp/10;   tmp %= 10;
      d[3] = tmp;
      uint32_t colors[4] = { pixel.Color(255,0,0), pixel.Color(0,255,0), pixel.Color(0,0,255), pixel.Color(255,255,0) };
      for (int i=0; i<4; ++i) {
        delay(150);
        blinkColor(colors[i], d[i], 60, 60);
      }
    }

    if (button_is_pressed(btn3)) {
      // Roll random number
      randomSeed(analogRead(1));
      int val = random(0, maxVal+1);
      int d[4];
      d[0] = val/1000; val %= 1000;
      d[1] = val/100;  val %= 100;
      d[2] = val/10;   val %= 10;
      d[3] = val;
      uint32_t colors[4] = { pixel.Color(255,0,0), pixel.Color(0,255,0), pixel.Color(0,0,255), pixel.Color(255,255,0) };
      for (int i=0; i<4; ++i) {
        if (i>0) delay(200);
        blinkColor(colors[i], d[i], 100, 80);
      }
      // Result pause, then allow reroll or exit
      delay(700);
    }
    if (button_is_pressed(btn4)) return;
  }
}

void bomb(){
    int interval = 1000;
    int minInterval = 75;
    int steps = 30;

    for (int i = 0; i < steps; i++){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(60);
        digitalWrite(LED_BUILTIN, LOW);

        delay(interval - 60);

        interval = interval - (interval - minInterval) / 6;
        if (interval < minInterval) interval = minInterval;
    }
    digitalWrite(Func1, HIGH);
    delay(20);
    digitalWrite(Func1, LOW);
}

void loop(){
  if (button_is_pressed(btn2)){
    selectedFunction++;
    indicateFunction(selectedFunction);
    if (selectedFunction > totalFunctions) selectedFunction = 1;
    delay(150);
  } else if (button_is_pressed(btn1)){
      selectedFunction--;
      indicateFunction(selectedFunction);
    if (selectedFunction < 1) selectedFunction = totalFunctions;
    delay(150);
  } else if (button_is_pressed(btn3)){
    switch (selectedFunction){
      case 1: outputs(); break;
      case 2: colorPicker(); break;
      case 3: bomb(); break;
      case 4: randomInt(); break;
      case 5: goToSleep(); break;
    }
  } else if (button_is_pressed(btn4)){
    reset();
  }
  delay(60);
}
