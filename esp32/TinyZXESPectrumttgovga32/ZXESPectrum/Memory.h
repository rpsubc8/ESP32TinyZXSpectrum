#ifndef _MEMORY_H
 #define _MEMORY_H
 #include <Arduino.h>
 
 #include "gbConfig.h"
 #ifdef use_lib_core_jsanchezv
  extern unsigned char * rom0_inRAM_jsanchezv;
  extern unsigned char * rom0_jsanchezv;
  extern unsigned char * rom1_jsanchezv;
  extern unsigned char * rom2_jsanchezv;
  extern unsigned char * rom3_jsanchezv;
 #endif 

 #ifdef use_lib_core_linkefong
  extern unsigned char *rom0;
  extern unsigned char *rom1;
  extern unsigned char *rom2;
  extern unsigned char *rom3;

  extern unsigned char *ram0;
  extern unsigned char *ram1;
  extern unsigned char *ram2;
  extern unsigned char *ram3;
  extern unsigned char *ram4;
  #ifdef use_lib_iram_video
   #ifdef use_lib_only_48k
    extern unsigned char ram5[16384]; //Solo 48K    
    extern unsigned char *ram7;    
   #else
    extern unsigned char ram5[16384];
    extern unsigned char ram7[16384];
   #endif
  #else
   extern unsigned char *ram5;
   extern unsigned char *ram7;
  #endif
  extern unsigned char *ram6;
 

  extern unsigned char rom_latch, bank_latch, video_latch, paging_lock;
  extern unsigned char sp3_mode, sp3_rom, rom_in_use;
  //extern volatile boolean writeScreen;
 #endif 

 #ifdef use_lib_core_jsanchezv
  void ReloadLocalCacheROMram_jsanchezv(void);
  #ifdef use_lib_48k_iram_jsanchezv
   void AssignStaticRam_jsanchezv(unsigned char *ptr_ram0,unsigned char *ptr_ram2,unsigned char *ptr_ram5);
  #else
  #endif 
 #endif 
#endif
