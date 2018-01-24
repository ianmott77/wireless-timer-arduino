#ifndef TIMING_EYE_H
#define TIMING_EYE_H

#include <Arduino.h>

class TimingEye{
    public:
        TimingEye(int, int);
        int distance(); 
        void init();
        int getLastDistance();
    private:
        int finishDistance = 0;
        bool distanceSet = false;
        int lastDistance = 0;
        int monitor = 0;
        int trigger = 0;
};

#endif