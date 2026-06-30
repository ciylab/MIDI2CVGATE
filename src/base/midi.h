/**
 * @file midi.h
 * @brief Gestion des entrées MIDI.
 *
 * Classe qui gère les handle midi.
 */
#ifndef MIDI_H
#define MIDI_H

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte number, byte value); 
void handleProgramChange(byte channel, byte number);
void handleClock();
void midi_init();
void midi_read();

#endif
