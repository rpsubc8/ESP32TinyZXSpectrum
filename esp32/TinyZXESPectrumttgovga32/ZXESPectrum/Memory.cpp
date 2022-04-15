//#include "def/msg.h"
#include <Arduino.h>
#include <esp_attr.h>
#include <sdkconfig.h>

#include "gbConfig.h"
#include "gbGlobals.h"

//void errorHalt(String);

//byte *rom0;
//JJ byte *rom1;
//JJ byte *rom2;
//JJ byte *rom3;
#ifdef use_lib_core_linkefong
 const unsigned char *rom0;
 const unsigned char *rom1;
 const unsigned char *rom2;
 const unsigned char *rom3;

 unsigned char *ram0;
 unsigned char *ram1;
 unsigned char *ram2;
 unsigned char *ram3;
 unsigned char *ram4;
 #ifdef use_lib_iram_video
  #ifdef use_lib_only_48k
   unsigned char ram5[16384]; //Solo 48k   
   unsigned char *ram7;
  #else
   unsigned char ram5[16384];
   unsigned char ram7[16384];
  #endif
 #else 
  unsigned char *ram5;
  unsigned char *ram7;
 #endif 
 unsigned char *ram6;

 volatile unsigned char bank_latch = 0;
 volatile unsigned char video_latch = 0;
 volatile unsigned char rom_latch = 0;
 volatile unsigned char paging_lock = 0;
 //volatile boolean writeScreen;
 unsigned char sp3_mode = 0;
 unsigned char sp3_rom = 0;
 unsigned char rom_in_use;
#endif 

#ifdef use_lib_core_jsanchezv
 uint8_t * rom0_inRAM_jsanchezv;
 const uint8_t * rom0_jsanchezv;
 const uint8_t * rom1_jsanchezv;
 const uint8_t * rom2_jsanchezv;
 const uint8_t * rom3_jsanchezv;

 //unsigned char * ram1_jsanchezv;
 #ifdef use_lib_48k_iram_jsanchezv
  static unsigned char * static_ram0_jsanchezv;
  static unsigned char * static_ram2_jsanchezv;
  static unsigned char * static_ram5_jsanchezv;
  
  void AssignStaticRam_jsanchezv(unsigned char *ptr_ram0,unsigned char *ptr_ram2,unsigned char *ptr_ram5)
  {
   static_ram0_jsanchezv = ptr_ram0;
   static_ram2_jsanchezv = ptr_ram2;
   static_ram5_jsanchezv = ptr_ram5;
  }
 #else
 #endif

 void ReloadLocalCacheROMram_jsanchezv()
 {
  gb_local_cache_rom_jsanchezv[0]=(uint8_t *)rom0_jsanchezv;
  gb_local_cache_rom_jsanchezv[1]=(uint8_t *)rom1_jsanchezv;
  gb_local_cache_rom_jsanchezv[2]=(uint8_t *)rom2_jsanchezv;
  gb_local_cache_rom_jsanchezv[3]=(uint8_t *)rom3_jsanchezv;
  
  
  gb_local_cache_ram_jsanchezv[1]=ram1_jsanchezv;  
  gb_local_cache_ram_jsanchezv[3]=ram3_jsanchezv;
  gb_local_cache_ram_jsanchezv[4]=ram4_jsanchezv;  
  gb_local_cache_ram_jsanchezv[6]=ram6_jsanchezv;
  gb_local_cache_ram_jsanchezv[7]=ram7_jsanchezv;  

  #ifdef use_lib_48k_iram_jsanchezv
   gb_local_cache_ram_jsanchezv[0]=static_ram0_jsanchezv;
   gb_local_cache_ram_jsanchezv[2]=static_ram2_jsanchezv;
   gb_local_cache_ram_jsanchezv[5]=static_ram5_jsanchezv;
  #else
   gb_local_cache_ram_jsanchezv[0]=ram0_jsanchezv;
   gb_local_cache_ram_jsanchezv[2]=ram2_jsanchezv;
   gb_local_cache_ram_jsanchezv[5]=ram5_jsanchezv;
  #endif
 }
#endif 
