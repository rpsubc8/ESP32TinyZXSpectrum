#ifndef __ZXMAIN_INCLUDED__
#define __ZXMAIN_INCLUDED__


#include "gbConfig.h"
#ifdef use_lib_core_linkefong

#include "z80user.h"

extern Z80_STATE _zxCpu;

//extern int gb_screen_xIni;
//extern int gb_screen_yIni;
extern unsigned char gbSoundSpeaker;
extern unsigned char gbSoundSpeakerBefore;
extern unsigned int gbContSPKChangeSamples;
//extern unsigned int gbContSPKSamples;

//#ifdef use_lib_sound_ay8912
// extern int gb_ay8912_reg_select;
// extern int gb_ay8912_reg_value;
// extern int gb_ay8912_A_frec_fine;
// extern int gb_ay8912_B_frec_fine;
// extern int gb_ay8912_C_frec_fine;
// extern int gb_ay8912_A_frec_course;
// extern int gb_ay8912_B_frec_course;
// extern int gb_ay8912_C_frec_course;
// extern int gb_ay8912_noise_pitch;
// extern int gb_ay8912_A_frec;
// extern int gb_ay8912_B_frec;
// extern int gb_ay8912_C_frec;
// extern int gb_ay8912_A_vol;
// extern int gb_ay8912_B_vol;
// extern int gb_ay8912_C_vol;
// extern int gb_ay8912_mixer;
// extern unsigned char gb_silence_all_channels;
// #define gb_frecuencia_ini 220
//#endif 

//#ifdef use_lib_mouse_kempston
// extern int gb_z80_mouse_x;
// extern int gb_z80_mouse_y;
// extern int gb_z80_mouseBtn;
//#endif


void zx_setup();
//JJ int32_t zx_loop();
void zx_loop(void);
void zx_reset();
void ReloadLocalCacheROMram(void);

extern "C" uint8_t fast_readbyte(uint16_t addr);
//JJ extern "C" uint8_t readbyte(uint16_t addr);
extern "C" void writebyte(uint16_t addr, uint8_t data);
extern "C" uint16_t readword(uint16_t addr);
extern "C" void writeword(uint16_t addr, uint16_t data);
//#ifdef use_lib_sound_ay8912
// extern "C" void ResetSound();
//#endif 


#endif

#endif
