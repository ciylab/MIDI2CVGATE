#include "Modules.h"

extern int cursorNum;

Modules::Modules() {
    int i;
    moduleNum = -1;
    newPage = true;
    this->size = 0;
    for(i = 0; i < 64; i++) {
        this->page[i] = ' ';
    }
    this->page[i] = '\0';
}

void Modules::updatePage(Module m) {
    int start = 8 * this->size + 1;
    Parameter::copy(m.name, this->page + start, 7);
}

void Modules::add(Module *m) {
    if(this->size < 8) {
        this->modules[this->size] = m;
        this->updatePage(*m);
        this->size++;
    }
}

Module *Modules::getCurrentModule() {
    if(moduleNum != -1) {
        return this->modules[moduleNum];
    }
    return NULL;
}

void Modules::setCurrentModule(int number) {
    this->moduleNum = -1;
    cursorNum = number % this->size;
}
