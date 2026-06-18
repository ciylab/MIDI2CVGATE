/**
 * @file Arp.h
 * @brief Gestion d'un arpégiateur/séquenceur.
 *
 * Possible transposition et filtrage dans une plage de valeur
 * pour les entrées.
 */
#ifndef ARP_H
#define ARP_H
#include "../base/Module.h"

class Arp: public Module {
    private:
        int pitchs[16];
        int size;

    public:
        /**
         * Constructeur avec une taille nulle pour le 
         * tableau des enregistrements.
         */
        Arp(char *name, uint8_t pin_out, uint8_t channel_in): 
            Module(name, pin_out, channel_in) {
                this->add(Parameter((char *) "IN", 0, 16, channel_in, 
                            Parameter::CHANNEL));
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
                this->add(Parameter((char *) "CHANGE", 0, 6, 0,
                            Parameter::BAR));
                this->size = 0;
                this->receive = false;
            }
        /**
         * Retourne un delta par rapport à la note jouée.
         */
        int randomize();
        void play();
        void update(int cursorNum);
        /**
         * Efface la séquence.
         */
        void del();
        void handleNoteOn(byte channel, byte pitch, byte velocity);
        void handleNoteOff(byte channel, byte pitch, byte velocity);
};


#endif
