/**
 * @file Encoder.h
 * @brief Gestion des deux encodeurs.
 *
 * Classe qui gère les deux encodeurs pour la rotation et la pression.
 */
#ifndef ENCODER_H
#define ENCODER_H
#include <Versatile_RotaryEncoder.h>
#define OUT_CLOCK A6

class Encoder {
    private:
        Versatile_RotaryEncoder *ve;

    public:
        /**
         * Constructeur avec initialisation de l'instance
         * et des pinModes.
         */
        Encoder(byte clk, byte dt, byte sw);
        /**
         * Gestion de la pression qui valide un changement de page.
         */
        static void handlePress_parameter();
        /**
         * Gestion d'une pression longue pour le reboot.
         */
        static void handleLongPress_parameter();
        /**
         * Gestion de la rotation qui sélectionne le module ou le 
         * paramètre.
         */
        static void handleRotate_parameter(int8_t rotation);
        /**
         * Gestion de la pression renvoyée à la rotation nulle. 
         * Activé uniquement pour le ON/OFF.
         */
        static void handlePress_value();
        /**
         * Gestion de la rotation qui modifie les valeurs en 
         * les affichant sauf au premier accès. 
         * Désactivé pour le ON/OFF. 
         */
        static void handleRotate_value(int8_t rotation);
        /**
         * Retourne le champ privé.
         */
        Versatile_RotaryEncoder *getVersatile_RotaryEncoder() {
            return ve;
        }
};

#endif

