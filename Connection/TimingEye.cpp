#include "TimingEye.h"

TimingEye::TimingEye(int mode, int monitor, int trigger){
    this->mode = mode;
    this->monitor = monitor;
    this->trigger = trigger;
    this->lidar = new LIDARLite();
}

void TimingEye::init(){
    if(this->mode == 1){
        pinMode(this->trigger, OUTPUT); // Set trigger pin
        digitalWrite(this->trigger, LOW); // Set trigger LOW for continuous read
        pinMode(this->monitor, INPUT); 
    }else if(this->mode == 2){
        this->lidar->configure(4);
    }
}

int TimingEye::distance(){
    if(this->mode == 1){
        int pulseWidth = pulseIn(this->monitor, HIGH);
        if(pulseWidth != 0){
            this->lastDistance = pulseWidth / 10;
        }
        return -1;
    }else if(this->mode == 2){
        this->lastDistance = (this->measurements % 99 == 0) ? this->lidar->distance(): this->lidar->distance(false);
    }
    this->measurements++;
    return this->lastDistance;
}

int TimingEye::quickDistance(){
    if(this->mode == 1){
        int pulseWidth = pulseIn(this->monitor, HIGH);
        if(pulseWidth != 0){
            return pulseWidth / 10;
        }
        return -1;
    }else if(this->mode == 2){
        this->measurements++;
        return ((this->measurements - 1) % 99 == 0) ? this->lidar->distance(): this->lidar->distance(false);
    }
}

int TimingEye::getLastDistance(){
    return this->lastDistance;
}