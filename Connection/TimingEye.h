#ifndef TIMING_EYE_H
#define TIMING_EYE_H

#include <Arduino.h>
#include <Wire.h>
#include "LIDARLite.h"
/*modes
1: PWM
2: I2C
*/
class TimingEye{
    public:
        TimingEye(int, int monitor = 0, int trigger = 0);
        int distance(); 
        void init();
        int getLastDistance();
        int quickDistance();
    private:
        int finishDistance = 0;
        bool distanceSet = false;
        int lastDistance = 0;
        int monitor = 0;
        int trigger = 0;
        int mode = 1;
        unsigned long measurements = 0;
        LIDARLite * lidar;
};

#endif