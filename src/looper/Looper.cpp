#include "Looper.h"
#include "../base/Modules.h"

extern unsigned long tick;

void Looper::update(int cursorNum) {
    if(cursorNum != 2) {
        this->parameters[2].value = 0;
    }
    if(cursorNum != 3) {
        this->parameters[3].value = 0;
    } else if(this->parameters[3].value) {
        del();
    }
    if(cursorNum == 6) {
        if(this->parameters[7].value < this->parameters[6].value) {
            this->parameters[6].value = this->parameters[7].value;
        }
    } else if(cursorNum == 7) {
        if(this->parameters[7].value < this->parameters[6].value) {
            this->parameters[7].value = this->parameters[6].value;
        }
    }
}

void Looper::handleNoteOn(byte channel, byte pitch, byte velocity) {
    if(this->parameters[0].value == channel && pitch <= NOTE_MAX) {
        Module::send_cv(1, (int) round(4095. * pitch / NOTE_MAX));
        digitalWrite(this->pin_out, HIGH);
        if(this->parameters[2].value) {
            this->pitchs[tick % (6 * this->parameters[1].value)] = pitch;
        } 
        this->receive = true;
    }
}

void Looper::handleNoteOff(byte channel, byte pitch, byte velocity) {
    if(this->parameters[0].value == channel && pitch <= NOTE_MAX) {
        digitalWrite(this->pin_out, LOW);
        this->receive = false;
    }
}

void Looper::del() {
    for (int i = 0; i < 6 * 16; i++) {
        this->pitchs[i] = NOTE_MAX + 1;
    }
}

void Looper::play() {
    int this_tick = tick % (6 * this->parameters[1].value);
    byte pitch;
    if(this->parameters[6].value <= pitchs[this_tick] && 
            pitchs[this_tick]<= this->parameters[7].value) {
        digitalWrite(this->pin_out, HIGH);
        pitch = pitchs[this_tick] + this->parameters[5].value;
        if(NOTE_MAX < pitch) {
            pitch = pitch - 12;
        }
        Module::send_cv(1, (int) round(4095. * pitch / NOTE_MAX));
        this->start_gate = tick;
    } else if((int) (tick - start_gate) == this->parameters[4].value) {
        digitalWrite(this->pin_out, LOW);
    }
}
