/**
 * @file MIDI2CVGATE.ino
 * @brief Fichier principal du projet.
 *
 * C'est le code d'un module Eurorack qui prend en entrée un signal
 * MIDI et donne 4 sorties LOW/HIGH et 2 sorties CV de 0 à 5 volts.
 * 
 * Entre les entrées/sorties, un micro-contrôleur va modifier les 
 * informations comme par exemple appliquer une transpostion. Les 
 * paramètres et leur valeur sont gérées par 2 encodeurs rotatifs
 * et affichées dans un écran oled 128x64 sur 4 lignes.
 * @author Pierrick MEIGNEN
 * @version 1.0
 * @date Le 26 février 2025
 */

/** base */
#include "src/base/Display.h"
#include "src/base/Modules.h"
#include "src/base/Encoder.h"
#include "src/base/midi.h"
/** modules */
#include "src/time/Time.h"
#include "src/drum/Drum.h"
#include "src/looper/Looper.h"
#include "src/arpeggiator/Arp.h"


/**
 * @brief Déclaration de l'écran.
 */
Display *oled = new Display();

/**
 * @brief Déclaration de l'encodeur PARAMETER avec les numéros de pins.
 */
Encoder parameter = Encoder(6, 7, 9);

/**
 * @brief Déclaration de l'encodeur VALUE avec les numéros de pins.
 */
Encoder value = Encoder(3, 2, 1);

/**
 * @brief Liste des modules (algorithmes).
 */
Modules ms = Modules();

/**
 * Vrai si l'horloge est générée par le micro-contrôleur.
 */
bool internalClock; 
unsigned long tick; /**<6 ticks par double-croche (PPQN)*/
unsigned long start; /**<Instant du dernier tick.*/
/**
 * Temps sépararant deux ticks en millisecondes.
 */
unsigned long delayBetweenTicks;

/**
 * Toutes les initialisations avec la création des modules.
 */
void setup() {
    internalClock = true;
    delayBetweenTicks = 60000 / 24 / 120;
    tick = 0;
    oled->init();
    oled->welcome();
    /* init pins out (led blue, blue, yellow, yellow, red)*/
    byte pins[] = {OUT_ARP, OUT_LOOPER, 
        OUT_DRUM_1, OUT_DRUM_2, OUT_CLOCK};
    for (byte i = 0; i < 5; i++) {
        digitalWrite(pins[i], HIGH);
        delay(500);
    }
    for (byte i = 0; i < 5; i++) {
        digitalWrite(pins[i], LOW);
    }
    oled->clear();
    ms.add(new Drum((char *)"DRUM 1", OUT_DRUM_1, 1));
    ms.add(new Drum((char *)"DRUM 2", OUT_DRUM_2, 2));
    ms.add(new Arp((char *)"ARP  1", OUT_ARP, 3));
    ms.add(new Looper((char *)"LOOP 2", OUT_LOOPER, 4));
    ms.add(new Time((char *)"TIME", OUT_CLOCK, 5));
    value.getVersatile_RotaryEncoder()->
        setHandlePress(Encoder::handlePress_value);
    value.getVersatile_RotaryEncoder()->
        setHandleRotate(Encoder::handleRotate_value); 
    parameter.getVersatile_RotaryEncoder()->
        setHandleRotate(Encoder::handleRotate_parameter);
    parameter.getVersatile_RotaryEncoder()->
        setHandlePress(Encoder::handlePress_parameter);           
    parameter.getVersatile_RotaryEncoder()->
        setHandleLongPress(Encoder::handleLongPress_parameter);
    midi_init();
    start = millis();
    SPI.begin();
    pinMode(SS, OUTPUT);
}

/**
 * Boucle principale :
 * - lire les encodeurs
 * - afficher les données
 * - jouer les modules pour l'horloge interne
 */
void loop() {
    value.getVersatile_RotaryEncoder()->ReadEncoder();
    parameter.getVersatile_RotaryEncoder()->ReadEncoder();
    oled->display();
    midi_read();
    // Condition qui réagit aux entrées de l'horloge interne.
    if(internalClock && millis() - start > delayBetweenTicks) {
        start += delayBetweenTicks;
        ms.play();
        tick++;
    }
}
