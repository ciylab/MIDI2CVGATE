/**
 * @file Drum.h
 * @brief Gestion du rythme.

 * Pour générer un rythme euclidien avec possibilité d'ajouter 
 * des beats par enregistrement par un tableau de booléens.
 *
 * Les beats enregistrés sont stockés dans un tableau
 * de ticks (24 par beats).
 */
#ifndef DRUM_H
#define DRUM_H
#include "../base/Module.h"

class Drum: public Module {
    private:
        bool triggers[96];

    public:
        /**
         * Initialise aussi le tableau des beats enregistrés.
         */
        Drum(char *name, uint8_t pin_out, uint8_t channel_in): 
            Module(name, pin_out, channel_in) {
                this->add(Parameter((char *) "IN", 0, 16, channel_in, 
                            Parameter::CHANNEL));
                this->add(Parameter((char *) "LENGTH", 1, 16, 16,
                            Parameter::NUM));
                this->add(Parameter((char *) "BEATS", 0, 16, 4,
                            Parameter::NUM));
                this->add(Parameter((char *) "SHIFT", 0, 16, 0,
                            Parameter::NUM));
                this->add(Parameter((char *) "RECORD", 0, 1, 0, 
                            Parameter::ONOFF));
                this->add(Parameter((char *) "DELETE", 0, 1, 0,
                            Parameter::ONOFF));
                this->add(Parameter((char *) "GATE", 1, 5, 2,
                            Parameter::BAR));
                this->add(Parameter((char *) "FINE", 0, 6, 3,
                            Parameter::CBAR));
                for (int i = 0; i < 96; i++) {
                    this->triggers[i] = false;
                }
                this->receive = false;
            }
        void play();
        void update(int cursorNum);
        /**
         * Pour effacer l'enregistrement.
         */
        void del();
        /**
         * Pour enregistrer.
         */
        void rec();
        void handleNoteOn(byte channel, byte pitch, byte velocity);
        void handleNoteOff(byte channel, byte pitch, byte velocity);
};

#endif
