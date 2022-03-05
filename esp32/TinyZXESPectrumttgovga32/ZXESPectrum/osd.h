#ifndef OSDFILE_H
 #define OSDFILE_H

// OSD Headers
#include <Arduino.h>

// Defines

// SNA Management

void changeSna(String sna_filename);
void changeSna2Flash(unsigned char id,unsigned char isSNA48K);
// ULA
//void stepULA();
void do_tinyOSD(void);
void SDLActivarOSDMainMenu();
void SDLOSDClear(void);
//void SDLEsperaTeclado(void);
void SDLClear(void);
void jj_fast_putpixel(int x,int y,unsigned char c);
int LoadBlocksTape(unsigned char idTape,int *arrayTape);
void Z80EmuSelectTape(unsigned char aux);

void SetMode48K(void);
void SetMode128K(void);

void ResetSound(void);
#endif
