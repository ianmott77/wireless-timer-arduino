#include <EEPROM.h>
#define DEVICE_ID 3

void setup(){
    EEPROM.write(0, DEVICE_ID);
}
void loop(){}