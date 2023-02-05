#ifndef OSDFILE_H
 #define OSDFILE_H

#include "gbConfig.h"

#ifdef use_lib_wifi
 #include <WiFi.h>
// #include <WiFiMulti.h>
 #include <HTTPClient.h> 
#endif 
// OSD Headers
#include <Arduino.h>

// Defines

// SNA Management

//void changeSna(String sna_filename); //No se usa
#ifdef use_lib_wifi
#else
 void changeSna2Flash(unsigned char id,unsigned char isSNA48K);  
#endif 
#ifdef use_lib_sna_uart
 unsigned short int Leer_stream_UART(unsigned short int topeLeer, unsigned short int topeTimeOut,unsigned char *isTimeOut);
 void changeSna2UART(unsigned char isSNA48K);
#endif 
#ifdef use_lib_wifi
 void changeSna2FlashFromWIFI(char *cadUrl,unsigned char isSNA48K);
#endif 
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

unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax);
#ifdef use_lib_wifi
 void OSDMenuRowsDisplayScrollFromWIFI(unsigned char *ptrBuffer,unsigned char currentId,unsigned char aMax);
 unsigned char ShowTinyMenuFromWIFI(const char *cadTitle,unsigned char *ptrBuffer,unsigned char aMax);
 //bool downloadURL(char const * URL, char const * filename); 
 //bool downloadURL(char const * URL, unsigned char * ptrDest, unsigned char isPtrDest);
 //void AsignarOSD_WIFI(WiFiMulti * ptr);
 //void AsignarOSD_WIFI(WiFiMulti * ptr, HTTPClient * ptr_http, WiFiClient * ptr_stream);
 //void AsignarOSD_WIFI(HTTPClient * ptr_http, WiFiClient * ptr_stream);
#endif 


#ifdef use_lib_keyboard_uart
 void keyboard_uart_poll(void);
#endif 

void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor);

#endif
