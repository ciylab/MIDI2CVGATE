#include "Module.h"

extern int cursorNum;
extern int bufferedCursorNum;
extern bool show;

Module::Module() {
    int i;
    this->size = 0;
    for(i = 0; i < 64; i++) {
        this->page[i] = ' ';
    }
    this->page[i] = '\0';
}

Module::Module(char name[8], uint8_t pin_out, uint8_t channel_in): Module() {
    Parameter::copy(name, this->name, 8);
    this->pin_out = pin_out;
    this->channel_in = channel_in;
    pinMode(this->pin_out, OUTPUT); 
    digitalWrite(this->pin_out, LOW);
}

void Module::updatePage(Parameter p) {
    int start = 8 * this->size + 1;
    int i = 0;
    while(i < 7 && p.name[i] != '\0') {
        this->page[start + i] = p.name[i];
        i++;
    } 
    while (i < 7) {
        this->page[start + i] = ' ';
        i++;
    }
}

void Module::add(Parameter p) {
    if(this->size < 8) {
        this->parameters[this->size] = p;
        this->updatePage(p);
        this->size++;
    }
}

void Module::handleControlChange(byte channel, byte number, byte value) {
    if(number == 1 || this->parameters[0].value != channel ||
            this->parameters[number - 1].type == Parameter::ONOFF) {
        return;
    }
    if(cursorNum != number - 1) {
        bufferedCursorNum = number - 1;
        show = false;
    } else if(!show) {
        show = true;
        return;
    }
    this->parameters[bufferedCursorNum].value = 
        map(value, 0, 127, this->parameters[bufferedCursorNum].min,
                this->parameters[bufferedCursorNum].max);
    this->update(bufferedCursorNum);
}
