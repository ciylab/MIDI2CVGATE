#/**
  * @file Time.h
  * @brief Classe pour gérer la sortie de l'horloge.
  */
#ifndef TIME_H
#define TIME_H
#include "../base/Module.h"

extern bool internalClock;

class Time : public Module {
    private:
        int period;

    public:
        /**
         * Avec une variable privée permettant de gérer les
         * subdivisions du beat.
         */
        Time(char *name, uint8_t pin_out, uint8_t channel_in): 
            Module(name, pin_out, channel_in) {
                this->add(Parameter((char *) "IN", 0, 16, channel_in, 
                            Parameter::CHANNEL));
                this->add(Parameter((char *) "CLOCK", 0, 1, internalClock, 
                            Parameter::ONOFF));
                this->add(Parameter((char *) "BPM", 30, 240, 120,
                            Parameter::NUM));
                this->add(Parameter((char *) "MULT", 1, 4, 1,
                            Parameter::NUM));
                this->add(Parameter((char *) "DIV", 1, 4, 1,
                            Parameter::NUM));
                this->period = 24;
                this->receive = false;
            }
        void handleNoteOn(byte channel, byte pitch, byte velocity) {}
        void handleNoteOff(byte channel, byte pitch, byte velocity) {}
        void update(int cursorNum);
        void play();
};


#endif
