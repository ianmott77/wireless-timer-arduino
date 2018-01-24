#include "TimingEye.h"

TimingEye::TimingEye(int monitor, int trigger){
    this->monitor = monitor;
    this->trigger = trigger;
}

void TimingEye::init(){
    pinMode(this->trigger, OUTPUT); // Set trigger pin
    digitalWrite(this->trigger, LOW); // Set trigger LOW for continuous read
    pinMode(this->monitor, INPUT); 
}

int TimingEye::distance(){
    int pulseWidth = pulseIn(this->monitor, HIGH);
    if(pulseWidth != 0){
        this->lastDistance = pulseWidth / 10;
        return this->lastDistance;
    }
    return -1;
}

int TimingEye::getLastDistance(){
    return this->lastDistance;
}