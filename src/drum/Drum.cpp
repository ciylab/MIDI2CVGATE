#include "Drum.h"
#include "../base/Modules.h"

extern unsigned long tick;

void Drum::update(int cursorNum) {
    if(cursorNum != 4) {
        this->parameters[4].value = 0;
    }
    if(cursorNum != 5) {
        this->parameters[5].value = 0;
    } else if(this->parameters[5].value) {
        del();
    }
}

void Drum::rec() {
    triggers[tick % (6 * this->parameters[1].value)] = true;
}

void Drum::handleNoteOn(byte channel, byte pitch, byte velocity) {
    if(this->parameters[0].value == channel) {
        digitalWrite(this->pin_out, HIGH);
        this->receive = true;
        if(this->parameters[4].value) {
            rec();
        } 
    }
}

void Drum::handleNoteOff(byte channel, byte pitch, byte velocity) {
    digitalWrite(this->pin_out, LOW);
    this->receive = false;
}

void Drum::del() {
    for (int i = 0; i < 96; i++) {
        triggers[i] = false;
    }
}

void Drum::play() {
    int length = this->parameters[1].value;
    int beats = this->parameters[2].value; 
    int shift = this->parameters[3].value;
    int gate = this->parameters[6].value;
    unsigned long the_tick = tick - this->parameters[7].value + 3;
    if(triggers[the_tick % (6 * length)]) {
        digitalWrite(this->pin_out, HIGH);
        this->start_gate = the_tick;
    } else if(the_tick % 6 == 0) {
        if((beats * (length - shift + (int) ((the_tick / 6) % length)) % 
                    length < beats)) {
            digitalWrite(this->pin_out, HIGH);
            this->start_gate = the_tick;
        }
    } else if((int) (the_tick - this->start_gate) == gate) {
        digitalWrite(this->pin_out, LOW);
    }
}
