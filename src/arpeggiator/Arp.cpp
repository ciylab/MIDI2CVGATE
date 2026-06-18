#include "Arp.h"
#include "../base/Modules.h"

extern unsigned long tick;

void Arp::update(int cursorNum) {
    switch(cursorNum) {
        case 2:
            if(this->parameters[2].value) {
                del();   
            }
            break;
        case 5:
            if(this->parameters[5].value > this->parameters[6].value) {
                this->parameters[5].value = this->parameters[6].value;
            }
            break;
        case 6:
            if(this->parameters[5].value > this->parameters[6].value) {
                this->parameters[6].value = this->parameters[5].value;
            }
    }
}

int Arp::randomize() {
    int delta[] = {-12, -5, 0, 7, 12};
    if(rand() % 6 < this->parameters[7].value) {
        return delta[rand() % 5];
    }
    return 0;
}

void Arp::handleNoteOn(byte channel, byte pitch, byte velocity) {
    if(this->parameters[0].value == channel && 
            pitch <= NOTE_MAX &&
            this->parameters[5].value <= pitch) {
        digitalWrite(this->pin_out, HIGH);
        if(this->parameters[1].value && this->size < 16) {
            this->pitchs[this->size] = pitch;
            this->size++;
        }
        pitch = 
            (pitch + 60 + this->parameters[4].value + randomize()) % 60; 
        Module::send_cv(0, (int) round(4095. * pitch / NOTE_MAX));
        this->receive = true;
    }
}

void Arp::handleNoteOff(byte channel, byte pitch, byte velocity) {
    if(this->parameters[0].value == channel) {
        digitalWrite(this->pin_out, LOW);
        this->receive = false;
    }
}

void Arp::del() {
    this->size = 0;
    digitalWrite(this->pin_out, LOW);
}

void Arp::play() {
    if(this->parameters[1].value) {
        return;
    }
    if(this->size == 0) {
        return;
    }
    if(tick % 6 == 0) {
        int this_tick = (tick / 6) % this->size;
        int pitch = pitchs[this_tick];
        if(this->parameters[5].value <= pitch && 
                pitch < this->parameters[6].value) {
            digitalWrite(this->pin_out, HIGH);
            pitch = (pitch + 60 + this->parameters[4].value + randomize()) % 60; 
            Module::send_cv(0, (int) round(4095. * pitch / NOTE_MAX));
            this->start_gate = tick;
        }
    } else if((int) (tick - start_gate) == this->parameters[3].value) {
        digitalWrite(this->pin_out, LOW);
    }
}
