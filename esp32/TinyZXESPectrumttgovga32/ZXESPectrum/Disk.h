#ifndef DISK_H
 #define DISK_H
// Defines
#define ASCII_NL 10
#define ON true
#define OFF false

// Headers
#include <Arduino.h>

// Declared vars
//extern unsigned char gb_cfg_arch_is48K;

// Declared methods
void load_rom2flash(unsigned char is48k,unsigned char id); //Para cargar rom de flash
void load_ram2Flash(unsigned char id,unsigned char isSNA48K);
#ifdef use_lib_core_linkefong
 void load_tape2Flash(unsigned char id);
#endif 
void load_scr2Flash(unsigned char id);
#endif
