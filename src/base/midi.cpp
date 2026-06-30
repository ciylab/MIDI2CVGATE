#include <MIDI.h>
#include "Encoder.h"
#include "Modules.h"
#include "Display.h"
#include "midi.h"

extern Modules ms;
extern bool internalClock; 
extern unsigned long tick; /**<6 ticks par double-croche (PPQN)*/
extern Display *oled;

/**
 * @brief Déclaration du pilote MIDI.
 */
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void midi_init() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    MIDI.setHandleClock(handleClock);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleControlChange(handleControlChange);
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
 * @brief Fonction qui réagit aux entrées de l'horloge externe (MIDI)
 * en jouant les modules sur les ticks.
 */
void handleClock() {
    if(internalClock) {
        return;
    }
    ms.play();
    tick++;
}

/**
 * Fonction qui simule une pression pour changer de module.
 */
void handleProgramChange(byte channel, byte number) {
    oled->clearCursor();
    ms.setCurrentModule(number);
    Encoder::handlePress_parameter();
}
    
void midi_read() {
    MIDI.read();
}
