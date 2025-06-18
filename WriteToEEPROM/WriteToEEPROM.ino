#include <EEPROM.h>

int NameAddr = 0;
int NumAddr = 100;

void setup() {}

void loop() {
  EEPROM.write(NameAddr, "Tim");
  EEPROM.write(NumAddr, "10");
  
}
