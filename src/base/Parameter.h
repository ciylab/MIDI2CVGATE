/**
 * @file Parameter.h
 * @brief Un élément constitutif des modules.
 * 
 * Pour définir un paramètre et initialiser
 * les valeurs min, max, par défaut et le type pour
 * l'affichage. 
 */
#ifndef PARAMETER_H
#define PARAMETER_H
#include <Arduino.h>

class Parameter {
    public:
        enum Type {NUM /**<numérique */, ONOFF /**<ON ou OFF */, 
            CHANNEL /**<canal MIDI (0 = OFF) */, BAR /**<Barre horiz. */, 
            KEY /**<Tonalité */, NOTE /**<Numéro MIDI */, 
            CBAR /**<Barre centrée */};

        char name[8];
        int min; /**<La valeur minimum du paramètre.*/
        int max; /**<La valeur maximum.*/
        int value; /**<La valeur courante.*/
        int defaultValue; /**<La valeur au démarrage.*/
        enum Type type; /**<Le type pour l'affichage.*/
        /**
         * Uniquement pour pouvoir écrire Parameter parameters[8]
         * dans la classe Module.
         */
        Parameter() {}
        /**
         * Constructeur pour tout type.
         */
        Parameter(char name[8], 
                int min, int max, 
                int defaultValue, 
                Type type);
        /**
         * Utile pour copier une chaîne en complétant avec des
         * espaces éventuellement.
         */
        static void copy(char src[8], char dest[8], int length);
};
#endif
