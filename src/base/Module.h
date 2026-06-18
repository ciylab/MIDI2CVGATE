/**
 * @file Module.h
 * @brief Modèle pour tous les modules instalables.
 */

#ifndef MODULE_H
#define MODULE_H
#include "Parameter.h"
#include <SPI.h>

#define SS 10
#define NOTE_MAX 59

/**
 * Sortie HIGH/LOW du trigger/gate numéro 1 sur le panel.
 */
#define OUT_DRUM_1 A3
/**
 * Sortie HIGH/LOW du trigger/gate numéro 2 sur le panel.
 */
#define OUT_DRUM_2 A7
/**
 * Sortie HIGH/LOW du gate numéro 1 en haut sur le panel.
 */
#define OUT_ARP     8
/**
 * Sortie HIGH/LOW du gate numéro 2 en haut sur le panel.
 */
#define OUT_LOOPER A0

/**
 * @class
 * La classe contient le texte affiché sur sa page ainsi que 
 * les paramètres.
 *
 * Les fonctions abstraites devant être implémentées par 
 * les classes filles sont déclarées ici. 
 */
class Module {
    private:
        int size;

    public:
        /** Le nom du module tel qu'à l'écarn.*/
        char name[8];
        /** La suite des noms.*/
        char page[65];
        /** Numéro du dernier tick. */
        unsigned long start_gate;
        /** Tableau des paramètres. */
        Parameter parameters[8];
        /** Numéro de pin de sortie pour le gate. */
        uint8_t pin_out;
        /** Numéro de canal MIDI en entrée. */
        uint8_t channel_in;
        /** Constructeur vide utilisé par défaut. */
        Module();
        /** 
         * @brief Constructeur avec son nom et son pin de sortie. 
         *
         * Les paramètres sont ajoutés ici avec leurs valeurs.
         */
        Module(char name[8], uint8_t pin_out, uint8_t channel_in);
        /**
         * Fonction qui construit la page en focntion des
         * paramètres ajoutés.
         */
        void updatePage(Parameter p);
        bool receive; /**< vrai si on joue une note on*/
        /** Fonction d'ajout d'un paramètre. */
        void add(Parameter p);
        /** Gère les notes on pour ce module. */
        virtual void handleNoteOn(byte, byte, byte){};
        /** Gère les notes off pour ce module. */
        virtual void handleNoteOff(byte, byte, byte){};
        /**
         * Gère les CC.
         */
        virtual void handleControlChange(byte, byte, byte);
        /** 
         * Pour mettre à jour les valeurs à partir du jeu
         * des encodeurs sur le numéro du curseur (choix).
         *
         * On applique ici les contraintes spécifiques 
         * au module.
         */
        virtual void update(int cursorNum){};
        /** Pour envoyer les signaux. */
        virtual void play(){};
        /** Ferme le gate = force note off. */
        void send_low(){
            digitalWrite(this->pin_out, LOW);
        };
        /**
         * Fonction du DAC.
         */
        static void send_cv(int ch, int cv) {
            digitalWrite(SS, LOW);
            if (ch == 0) {
                SPI.transfer((cv >> 8) | 0x30);  // H0x30=OUTA/1x
            } else if(ch == 1) {
                SPI.transfer((cv >> 8) | 0xB0);  // H0xB0=OUTB/1x
            }
            SPI.transfer(cv & 0xff);
            digitalWrite(SS, HIGH);
        }
        /**
         * @brief Retourne le nombre de paramètres. 
         *
         * Utile pour la navigation du curseur.
         */
        int getSize() {
            return this->size;
        }
};
#endif
