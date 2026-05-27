#include "Time.h"
#include "../base/Modules.h"

#define OUT_CLOCK A6

extern unsigned long tick;
extern unsigned long delayBetweenTicks;
extern unsigned long start;

void Time::play() {
    if(tick % period == 0) {
        digitalWrite(OUT_CLOCK, HIGH);
    } else if(tick % period == 1) {
        digitalWrite(OUT_CLOCK, LOW);
    }
}

void Time::update(int cursorNum) {
    switch(cursorNum) {
        case 1:
            internalClock = this->parameters[1].value;
            digitalWrite(OUT_CLOCK, LOW);
            digitalWrite(OUT_DRUM_1, LOW);
            digitalWrite(OUT_DRUM_2, LOW);
            digitalWrite(OUT_ARP, LOW);
            digitalWrite(OUT_LOOPER, LOW);
            if(internalClock) {
                start = millis();
            }
            break;
        case 2:
            if(internalClock) {
                delayBetweenTicks = 
                    60000 / 24 / this->parameters[2].value;
            } 
            break;
        case 3:
            period = 24;
            this->parameters[4].value = 1;
            if(this->parameters[3].value > 1) {
                period /= this->parameters[3].value;
            }
            break;
        case 4:
            period = 24;
            this->parameters[3].value = 1;
            if (this->parameters[4].value > 1) {
                period *= this->parameters[4].value;
            }
            break;
    }
}
