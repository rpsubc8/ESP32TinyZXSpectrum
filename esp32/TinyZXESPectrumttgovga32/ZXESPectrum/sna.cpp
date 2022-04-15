#include "gbConfig.h"
#include "gbGlobals.h"
#include "dataFlash/gbsna.h"
#include "Disk.h"
#include "PS2Kbd.h"
#include "z80main.h"
#include "ZXESPectrum.h"
#include "osd.h"

#ifdef use_lib_core_jsanchezv
 #include "jsanchezv_z80sim.h"
#endif 


#ifdef use_lib_wifi
#else
 //Carga SNA desde FLASH
 void changeSna2Flash(unsigned char id,unsigned char isSNA48K)
 {
  #ifdef use_lib_core_jsanchezv
   zx_reset_jsanchezv();
   gb_ptrSim->load_ram2Flash_jsanchezv(id,isSNA48K);
  #else
   //osdCenteredMsg((String)MSG_LOADING + ": " + (String)gb_list_sna_48k_title[id], LEVEL_INFO);
   zx_reset();
   //Serial.printf("FLASH SNA: %s\n", );
   //load_ram((String)DISK_SNA_DIR + "/" + sna_filename);
   load_ram2Flash(id,isSNA48K);
   //JJ osdCenteredMsg(MSG_SAVE_CONFIG, LEVEL_WARN);
   //JJ cfg_ram_file = (String)gb_list_sna_48k_title[id];
  #endif
 }
#endif 

#ifdef use_lib_wifi
 void changeSna2FlashFromWIFI(char * cadUrl,unsigned char isSNA48K)
 {
  #ifdef use_lib_core_jsanchezv
   zx_reset_jsanchezv();
   //gb_ptrSim->load_ram2Flash_jsanchezv(id,isSNA48K);
   gb_ptrSim->load_ram2Flash_jsanchezvFromWIFI(cadUrl,isSNA48K);
  #else 
   zx_reset();
   load_ram2FlashFromWIFI(cadUrl,isSNA48K);
  #endif
 }
#endif
