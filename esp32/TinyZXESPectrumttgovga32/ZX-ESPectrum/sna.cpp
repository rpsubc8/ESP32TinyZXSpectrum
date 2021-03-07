#include "dataFlash/gbsna.h"
#include "Disk.h"
#include "Emulator/Keyboard/PS2Kbd.h"
#include "Emulator/z80main.h"
#include "ZX-ESPectrum.h"
#include "osd.h"


//Carga SNA desde FLASH
void changeSna2Flash(unsigned char id,unsigned char isSNA48K)
{
 //osdCenteredMsg((String)MSG_LOADING + ": " + (String)gb_list_sna_48k_title[id], LEVEL_INFO);
 zx_reset();
 //Serial.printf("FLASH SNA: %s\n", );
 //load_ram((String)DISK_SNA_DIR + "/" + sna_filename);
 load_ram2Flash(id,isSNA48K);
 //JJ osdCenteredMsg(MSG_SAVE_CONFIG, LEVEL_WARN);
 //JJ cfg_ram_file = (String)gb_list_sna_48k_title[id];
}
