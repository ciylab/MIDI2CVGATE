#include "Display.h"
#include "Modules.h"
#include <U8x8lib.h>
#include "my_u8x8_font_7x14_1x2_r.h"

/**
 * @brief Déclaration de l'écran. 
 *
 * La variable u8x8 est encapsulée ailleurs.
 */ 

#ifdef SSD1306
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
#elif SH1106
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
#endif

extern Modules ms;
extern bool show;
/**
 * Permet de savoir quel est le module ou le paramètre courant.
 */
int cursorNum;
/**
 * Pour déplacer le curseur en temps réel.
 */
int bufferedCursorNum;

Display::Display() {
    int i;
    begin = 0;
    end = 64;
    for (i = 0; i < 64; i++) {
        screen[i] = ' ';
        buffer[i] = ' ';
    }
    screen[i] = '\0';
    buffer[i] = '\0';
}

void Display::init() {
    u8x8.begin();
    u8x8.setFont(my_u8x8_font_7x14_1x2_r);
    cursorNum = -1;
    bufferedCursorNum = 0;
}

void Display::writeChar(int position, char c) {
    u8x8.drawGlyph(position % 8 + 8 * (position / 32), 
            2 * ((position % 32) / 8), c);
}

void Display::welcome() {
    char temp[16];
    u8x8.drawString(0, 0, " MIDI 2");
    u8x8.drawString(0, 2, " CV/GATE");
    u8x8.drawString(0, 4, " BY CIYLAB");
    sprintf(temp, " %s",  VERSION);
    u8x8.drawString(0, 6, temp);
}

void Display::clear() {
    u8x8.clear();
}

void Display::setBuffer(char str[64]) {
    for(int i = 0; i < 64; i++) {
        buffer[i] = str[i];
    }
    begin = 0;
    end = 64;
}

void Display::nextBegin() {
    while(begin < end && screen[begin] == buffer[begin]) {
        begin++;
    }
}

void Display::clearCursor() {
    u8x8.drawGlyph(
            8 * (cursorNum / 4),
            2 * (cursorNum % 4), 
            ' ');
}

void Display::showCursor() {
    u8x8.drawGlyph(
            8 * (cursorNum / 4),
            2 * (cursorNum % 4), 
            CURSOR);
}

void Display::moveCursor() {
    clearCursor();
    cursorNum = bufferedCursorNum;
    showCursor();
}

void Display::loadPage() {
    showCursor();
    if(ms.moduleNum == -1) {
        setBuffer(ms.page);
    } else {
        setBuffer(ms.getCurrentModule()->page);
    }
}

void Display::progressBar(char bar[8], int level) {
    for(int i = 0; i < 7; i++) {
        if(i < level) {
            bar[i] = '\'';
        } else {
            bar[i] = ' ';
        }
    }
}

void Display::progressCenteredBar(char bar[8], int level) {
    for(int i = 0; i < 7; i++) {
        if((3 <= i && i < level) || (level <= i && i < 3)) {
            bar[i] = '\'';
        } else {
            bar[i] = ' ';
        }
    }
}

void Display::showValue() {
    char temp[8];
    char notes[][3] = {" C", "C#", " D", "D#", " E", " F", "F#", 
        " G", "G#", " A", "A#", " B"};
    Module *m = ms.getCurrentModule();
    begin = 1 + 8 * cursorNum;
    end = begin + 7;
    switch(m->parameters[cursorNum].type) {
        case Parameter::ONOFF:
            if(m->parameters[cursorNum].value == 0) {
                sprintf(temp, "%s", "OFF    ");
            } else {
                sprintf(temp, "%s", "ON     ");
            }
            break;
        case Parameter::CHANNEL:
            if(m->parameters[cursorNum].value == 0) {
                sprintf(temp, "%s", "OFF    ");
            } else {
                sprintf(temp, "%3d    ",
                        m->parameters[cursorNum].value);
            }
            break;
        case Parameter::BAR:
            progressBar(temp, 
                    m->parameters[cursorNum].value);
            break;
        case Parameter::CBAR:
            progressCenteredBar(temp, 
                    m->parameters[cursorNum].value);
            break;
        case Parameter::KEY:
            sprintf(temp, "%s    ", 
                    notes[m->parameters[cursorNum].value]);
            break;
        case Parameter::NOTE:
            sprintf(temp, "%s%d   ",
                    notes[m->parameters[cursorNum].value % 12],
                    m->parameters[cursorNum].value / 12);
            break;
        default:
            sprintf(temp, "%3d    ", 
                    m->parameters[cursorNum].value);
    }
    for(int i = 0; i < 7; i++) {
        buffer[begin + i] = temp[i];
    }
}

void Display::showName() {
    begin = 1 + 8 * cursorNum;
    end = begin + 7;
    for(int i = 0; i < 7; i++) {
        buffer[begin + i] = 
            ms.getCurrentModule()->parameters[cursorNum].name[i];
    }
}

void Display::display() {
    if(bufferedCursorNum != cursorNum) {
        if(ms.moduleNum != -1) {
            Display::showName();
        }
        Display::moveCursor();
        return;
    }
    if(ms.newPage) {
        ms.newPage = false;
        Display::loadPage();
        return;
    } 
    if(ms.moduleNum != -1 && show) {
        Display::showValue();     
    }
    if(begin == end) {
        return;
    }
    nextBegin();
    if(begin == end) { 
        return;
    }
    screen[begin] = buffer[begin];
    writeChar(begin, screen[begin]);
    begin++;
}
