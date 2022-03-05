#ifndef OSDFILE_H
 #define OSDFILE_H

// OSD Headers
#include <Arduino.h>

// Defines

// SNA Management

void changeSna(String sna_filename);
void changeSna2Flash(unsigned char id,unsigned char isSNA48K);
// ULA
void stepULA();
void do_tinyOSD(void);
void SDLActivarOSDMainMenu();
void SDLOSDClear(void);
//void SDLEsperaTeclado(void);
#endif
