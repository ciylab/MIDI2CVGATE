/**
 * @file Looper.h
 * @brief Gestion du looper.
 *
 * Le looper offre la possibilité d'enregistrer des notes et
 * de les jouer en boucle.
 */

#ifndef LOOPER_H
#define LOOPER_H
#include "../base/Module.h"

class Looper: public Module {
    private:
        byte pitchs[6 * 16];

    public:
        /**
         * Constructeur pour la gestion des notes et des gates.
         */
        Looper(char *name, uint8_t pin_out, uint8_t channel_in): 
            Module(name, pin_out, channel_in) {
                this->add(Parameter((char *) "IN", 0, 16, channel_in, 
                            Parameter::CHANNEL));
                this->add(Parameter((char *) "LENGTH", 1, 16, 16,
                            Parameter::NUM));
                this->add(Parameter((char *) "RECORD", 0, 1, 0, 
                            Parameter::ONOFF));
                this->add(Parameter((char *) "DELETE", 0, 1, 0,
                            Parameter::ONOFF));
                this->add(Parameter((char *) "GATE", 1, 5, 2,
                            Parameter::BAR));
                this->add(Parameter((char *) "KEY", 0, 11, 0,
                            Parameter::KEY));
                this->add(Parameter((char *) "MIN", 0, NOTE_MAX, 0,
                            Parameter::NOTE));
                this->add(Parameter((char *) "MAX", 0, NOTE_MAX, NOTE_MAX,
                            Parameter::NOTE));
                for (int i = 0; i < 6 * 16; i++) {
                    this->pitchs[i] = NOTE_MAX + 1;
                }
                this->receive = false;
            }
        void play();
        void update(int cursorNum);
        /**
         * Efface les données de la séquence.
         */
        void del();
        void handleNoteOn(byte channel, byte pitch, byte velocity);
        void handleNoteOff(byte channel, byte pitch, byte velocity);
};


#endif
