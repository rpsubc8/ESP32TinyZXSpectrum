#ifndef MEMORY_H
 #define MEMORY_H
 #include <Arduino.h>
 
 #include "gbConfig.h"
 #ifdef use_lib_core_jsanchezv
  extern byte * rom0_jsanchezv;
  extern byte * rom1_jsanchezv;
  extern byte * rom2_jsanchezv;
  extern byte * rom3_jsanchezv;
 #endif 

 extern byte *rom0;
 extern byte *rom1;
 extern byte *rom2;
 extern byte *rom3;

 extern byte *ram0;
 extern byte *ram1;
 extern byte *ram2;
 extern byte *ram3;
 extern byte *ram4;
 extern byte *ram5;
 extern byte *ram6;
 extern byte *ram7;

 extern byte rom_latch, bank_latch, video_latch, paging_lock;
 extern byte sp3_mode, sp3_rom, rom_in_use;
 extern volatile boolean writeScreen;

 #ifdef use_lib_core_jsanchezv
  void ReloadLocalCacheROMram_jsanchezv(void);
 #endif 
#endif
