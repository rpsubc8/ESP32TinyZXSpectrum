//#include "def/msg.h"
#include <Arduino.h>
#include <esp_attr.h>
#include <sdkconfig.h>

#include "gbConfig.h"
#include "gb_globals.h"

void errorHalt(String);

//byte *rom0;
//JJ byte *rom1;
//JJ byte *rom2;
//JJ byte *rom3;
const uint8_t *rom0;
const uint8_t *rom1;
const uint8_t *rom2;
const uint8_t *rom3;


byte *ram0;
byte *ram1;
byte *ram2;
byte *ram3;
byte *ram4;
byte *ram5;
byte *ram6;
byte *ram7;

volatile byte bank_latch = 0;
volatile byte video_latch = 0;
volatile byte rom_latch = 0;
volatile byte paging_lock = 0;
volatile boolean writeScreen;
byte sp3_mode = 0;
byte sp3_rom = 0;
byte rom_in_use;

#ifdef use_lib_core_jsanchezv
 const uint8_t * rom0_jsanchezv;
 const uint8_t * rom1_jsanchezv;
 const uint8_t * rom2_jsanchezv;
 const uint8_t * rom3_jsanchezv;

 void ReloadLocalCacheROMram_jsanchezv()
 {
  gb_local_cache_rom_jsanchezv[0]=(uint8_t *)rom0_jsanchezv;
  gb_local_cache_rom_jsanchezv[1]=(uint8_t *)rom1_jsanchezv;
  gb_local_cache_rom_jsanchezv[2]=(uint8_t *)rom2_jsanchezv;
  gb_local_cache_rom_jsanchezv[3]=(uint8_t *)rom3_jsanchezv;
  
  gb_local_cache_ram_jsanchezv[0]=ram0_jsanchezv;  
  gb_local_cache_ram_jsanchezv[1]=ram1_jsanchezv;
  gb_local_cache_ram_jsanchezv[2]=ram2_jsanchezv;
  gb_local_cache_ram_jsanchezv[3]=ram3_jsanchezv;
  gb_local_cache_ram_jsanchezv[4]=ram4_jsanchezv;
  gb_local_cache_ram_jsanchezv[5]=ram5_jsanchezv;
  gb_local_cache_ram_jsanchezv[6]=ram6_jsanchezv;
  gb_local_cache_ram_jsanchezv[7]=ram7_jsanchezv;  
 }
#endif 