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

#include <MIDI.h>
/** base */
#include "src/base/Display.h"
#include "src/base/Modules.h"
#include "src/base/Encoder.h"
/** modules */
#include "src/time/Time.h"
#include "src/drum/Drum.h"
#include "src/looper/Looper.h"
#include "src/arpeggiator/Arp.h"

/**
 * @brief Déclaration du pilote MIDI.
 */
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

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
 * @brief Fonction qui réagit aux entrées de l'horloge externe (MIDI)
 * en jouant les modules sur les ticks.
 */
void handleClock() {
    if(internalClock) {
        return;
    }
    play();
    tick++;
}

/**
 * @brief Boucle qui joue tous les modules sur les ticks d'horloge.
 */
void play() {
    for(int i = 0; i < ms.getSize(); i++) {
        ms.modules[i]->play();
    }
}

/**
 * Fonction qui réagit aux entrées de Note On.
 */
void handleNoteOn(byte channel, byte pitch, byte velocity) {
    if(ms.getCurrentModule() != NULL) {
        ms.getCurrentModule()->handleNoteOn(channel, pitch, velocity);
    }
}

/**
 * Fonction qui réagit aux entrées de Note Off.
 */
void handleNoteOff(byte channel, byte pitch, byte velocity) {
    if(ms.getCurrentModule() != NULL) {
        ms.getCurrentModule()->handleNoteOff(channel, pitch, velocity);
    }
}

/**
 * Fonction qui simule une pression pour changer de module.
 */
void handleProgramChange(byte channel, byte number) {
    oled->clearCursor();
    ms.setCurrentModule(number);
    Encoder::handlePress_parameter();
}

/**
 * Fonction qui modifie les paramètres par CC.
 */
void handleControlChange(byte channel, byte number, byte value) {
    if(ms.getCurrentModule() == NULL) {
        return;
    }
    if (number == 0 || ms.getCurrentModule()->getSize() < number) {
        return;
    }
    ms.getCurrentModule()->handleControlChange(channel, number, value);
}

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
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    MIDI.setHandleClock(handleClock);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleControlChange(handleControlChange);
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
    MIDI.read();
    // Condition qui réagit aux entrées de l'horloge interne.
    if(internalClock && millis() - start > delayBetweenTicks) {
        start += delayBetweenTicks;
        play();
        tick++;
    }
}
