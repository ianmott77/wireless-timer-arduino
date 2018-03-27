#include <EEPROM.h>
#define DEVICE_TYPE 3

void setup(){
    EEPROM.write(1, DEVICE_TYPE);
}
void loop(){}