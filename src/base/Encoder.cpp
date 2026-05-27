#include "Modules.h"
#include "Encoder.h"
#include "Display.h"

extern Modules ms;
extern int cursorNum;
extern int bufferedCursorNum;
/** Vrai si on affiche la valeur du paramètre.*/
bool show;

Encoder::Encoder(byte clk, byte dt, byte sw) {
    pinMode(clk, INPUT_PULLUP);
    pinMode(dt, INPUT_PULLUP);
    pinMode(sw, INPUT_PULLUP);
    this->ve = new Versatile_RotaryEncoder(clk, dt, sw);
}

void Encoder::handlePress_parameter() {
    if(ms.moduleNum == -1 &&
            ms.modules[cursorNum]->getSize() != 0) {
        ms.moduleNum = cursorNum;
        show = false;
    } else {
        if(ms.getCurrentModule()->receive) {
            ms.getCurrentModule()->send_low();
        }
        ms.moduleNum = -1;
    }
    bufferedCursorNum = 0;
    ms.newPage = true;
}

void Encoder::handleLongPress_parameter() {
    asm volatile("jmp 0x00");
}

void Encoder::handleRotate_parameter(int8_t rotation) {
    if(0 < rotation) {
        bufferedCursorNum++;
    } else {
        bufferedCursorNum--;
    }
    if(ms.moduleNum == -1) {
        bufferedCursorNum = 
            (bufferedCursorNum + ms.getSize()) % ms.getSize();
    } else {
        bufferedCursorNum =
            (bufferedCursorNum + ms.getCurrentModule()->getSize()) %
            ms.getCurrentModule()->getSize();
        show = false;
    }
}

void Encoder::handlePress_value() {
    Encoder::handleRotate_value(0);
}

void Encoder::handleRotate_value(int8_t rotation) {
    Module *m = ms.getCurrentModule();
    if(ms.moduleNum == -1) {
        return;
    }
    if(!show) {
        show = true;
        return;
    }
    if(rotation != 0 && 
            m->parameters[cursorNum].type == Parameter::ONOFF) {
        return;
    }
    if(0 < rotation && 
            m->parameters[cursorNum].value < 
            m->parameters[cursorNum].max) {
        m->parameters[cursorNum].value++;
    } else if(rotation < 0 && m->parameters[cursorNum].min < 
            m->parameters[cursorNum].value) {
        m->parameters[cursorNum].value--;
    } else if(rotation == 0 && 
            m->parameters[cursorNum].type == 
            Parameter::ONOFF) {
        m->parameters[cursorNum].value = 
            1 - m->parameters[cursorNum].value;
    }
    m->update(cursorNum);
}

