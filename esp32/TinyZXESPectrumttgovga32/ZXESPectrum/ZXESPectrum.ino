//Mod ackerman:
// - All use SRAM. Mode 128 KB NO PSRAM
// - Support AMX and Mouse Kempston (library Rob Kent jazzycamel)
// - Tiny OSD
// - Support core Lin Ke-Fong and JLS (Jose Luis Sanchez)
// - AY8912 dirty sound 3 channel (fabgl mod include project)
// - Speaker resample and mixer with AY8912 (fabgl)
// - SCR file load
// - File Tape BASIC load (only core Lin Ke-Fong)
// - File Tape SCREEN mem load
// - Out MIC tape sound to speaker
// - Remove sna save
// - Remove EAR and MIC tape hardware pinout
// - Remove WII pad
// - Mode thread or no thread (single core)
// - Mode 320x200, 320x240 and 360x200
// - Tiny bitluni library
// - Skip frame thread or delay video no thread
// - Screen offset X,Y
// - Mode bright compile
// - Mode 8 and 64 colors
// - Keyboard remap Shift+6, shift+7 and supr
// - Optimice video draw and CPU
// - Compatible Arduino IDE and PlatformIO
// - Customized parameters gbConfig.h
// - Arduino IDE 1.8.11 Espressif System 1.0.6
// - Library Arduino fabgl 0.9.0 sound include project
// - Library Arduino tiny bitluni 0.3.3 include project
// - Arduino support espressif
// - Visual Studio 1.66.1 PLATFORMIO 2.4.3 Espressif32 v3.5.0(python 3.6)
// - Compatible TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)
// - Tiny Bitluni 0.3.3 mod C fix 8 colors (Ricardo Massaro)
// - Tiny mouse library Arduino mod fix (Rob Kent jazzycamel)
// - Support WIFI Load SCR, SNA only 48k mode
//
// Basado en codigo de David Crespo ZX-ESPectrum-Wiimote
// -------------------------------------------------------------------
//  ESPectrum Emulator
//  Ramón Martínez & Jorge fuertes 2019
//  Though from ESP32 Spectrum Emulator (KOGEL esp32) Pete Todd 2017
// -------------------------------------------------------------------

#include "gbConfig.h"

#ifdef use_lib_sound_ay8912
 #include "fabgl.h" //Para fabgl
 #include "fabutils.h" //Para fabgl 
#endif 

//#ifdef use_lib_mouse_kempston
 //#include "fabgl.h" //Para fabgl
 //jjvga #include "WiFiGeneric.h"; //Fix WiFiGeneric.h No such file or directory
 //#include "fabutils.h" //Para fabgl
 //#include <mouse.h>
//#endif

#include "gbOptimice.h"
#include "dataFlash/gbrom.h"
#include "dataFlash/gbsna.h"
#include "dataFlash/gbtape.h"
#include "dataFlash/gbscr.h"
#include "PS2Kbd.h"
#include "Memory.h"
#include "clock.h"
#include "z80emu.h"
#include "z80main.h"
#include "z80user.h"
//#include "Disk.h"
#include <Arduino.h>
//Para ahorrar memoria
//JJ #include <esp_bt.h>

//#include "MartianVGA.h"

//#include "def/Font.h"
#include "hardware.h"
#include "driver/timer.h"
#include "soc/timer_group_struct.h"
#include "osd.h"
#include "gbGlobals.h"

#ifdef use_lib_wifi
 #include "gbWifi.h"
 #include <WiFi.h>
 //#include <WiFiMulti.h>
 #include <HTTPClient.h>
 #include "gbWifiConfig.h"
 //#include <esp_wifi.h>
 //WiFiMulti wifiMulti;
 HTTPClient http;
 WiFiClient * stream;
 
 unsigned char gb_buffer_wifi[1024]; //128 ficheros * 8
 int gb_size_file_wifi=0; 
#endif


#ifdef use_lib_mouse_kempston
 #include "PS2Mouse.h"
#endif

#ifdef use_lib_core_jsanchezv
 #include "jsanchezv_z80sim.h"
 #include "Memory.h"
 //#include "gbPrograma.h"
#endif


//Punteros a roms globales
const char ** gb_ptr_list_roms_48k_title= gb_list_roms_48k_title;
const unsigned char ** gb_ptr_list_roms_48k_data= gb_list_roms_48k_data;
const char ** gb_ptr_list_roms_128k_title= gb_list_roms_128k_title;
const array4rom * gb_ptr_list_roms_128k_data= gb_list_roms_128k_data;
//const unsigned char ** gb_ptr_list_roms_128k_data= gb_list_roms_128k_data;
unsigned char gb_max_list_rom_48= max_list_rom_48;
unsigned char gb_max_list_rom_128= max_list_rom_128;

//Punteros globales a SNA
const char ** gb_ptr_list_sna_48k_title= gb_list_sna_48k_title;
const unsigned char ** gb_ptr_list_sna_48k_data= gb_list_sna_48k_data;
const char ** gb_ptr_list_sna_128k_title= gb_list_sna_128k_title;  
const unsigned char ** gb_ptr_list_sna_128k_data= gb_list_sna_128k_data;
unsigned char gb_max_list_sna_48= max_list_sna_48; //Maximo 256
unsigned char gb_max_list_sna_128= max_list_sna_128; //Maximo 256
//Punteros globales Cintas
const char ** gb_ptr_list_tapes_48k_title= gb_list_tapes_48k_title;
const int * gb_ptr_list_tapes_48k_size= gb_list_tapes_48k_size;
const unsigned char ** gb_ptr_list_tapes_48k_data= gb_list_tapes_48k_data;
const char ** gb_ptr_list_tapes_128k_title= gb_list_tapes_128k_title;
const unsigned char ** gb_ptr_list_tapes_128k_data= gb_list_tapes_128k_data;
unsigned char gb_max_list_tape_48= max_list_tape_48; //Maximo 256
unsigned char gb_max_list_tape_128= max_list_tape_128; //Maximo 256
//Punteros globales scr
const char ** gb_ptr_list_scr_48k_title= gb_list_scr_48k_title;
const unsigned char ** gb_ptr_list_scr_48k_data= gb_list_scr_48k_data;
unsigned char gb_max_list_scr_48= max_list_scr_48; //Maximo 256


#include "vga_6bit.h"

#ifdef use_lib_vga8colors
  //DAC 3 bits 8 colores
  // 3 bit pins  
 static const unsigned char pin_config[] = {  
  PIN_RED_HIGH,
  PIN_GREEN_HIGH,  
  PIN_BLUE_HIGH,
  255,
  255,
  255,
  PIN_HSYNC,
  PIN_VSYNC
 };
#else
 //DAC 6 bits 64 colores
 static const unsigned char pin_config[] = {
  PIN_RED_LOW,
  PIN_RED_HIGH,
  PIN_GREEN_LOW,
  PIN_GREEN_HIGH,
  PIN_BLUE_LOW,
  PIN_BLUE_HIGH,
  PIN_HSYNC,
  PIN_VSYNC
 };
#endif

#ifdef use_lib_vga_thread
 volatile unsigned char gb_draw_thread= 1;
 volatile unsigned char gbFrameSkipVideoMaxCont = gb_frame_crt_skip;
#else
 unsigned char gbFrameSkipVideoMaxCont = gb_frame_crt_skip;  
#endif

unsigned char gb_delay_tick_cpu_micros= use_lib_delay_tick_cpu_micros;
unsigned char gb_auto_delay_cpu= use_lib_delay_tick_cpu_auto;

unsigned char gb_sync_bits;
unsigned char ** gb_buffer_vga;
unsigned int **gb_buffer_vga32;


int gb_screen_xIni=0;
int gb_screen_yIni=0;
unsigned char gbSoundSpeaker=0;
unsigned char gbSoundSpeakerBefore=0;

unsigned char gb_current_tape=0;
int gb_contTape=3;
int gb_tape_read=0;
int gb_local_arrayTape[32]; //32 bloques de cinta maximo
int gb_local_numBlocks=0; //Numero bloques cinta actual

#ifdef use_lib_vga_thread
#else
 unsigned char gbDelayVideo=20;
#endif



#ifdef use_lib_sound_ay8912
 SoundGenerator soundGenerator;
 SineWaveformGenerator gb_sineArray[4];
 unsigned char gbVolMixer_before[4]={0,0,0,0}; 
 short int gbFrecMixer_before[4]={0,0,0,0};
 unsigned char gbVolMixer_now[4]={0,0,0,0};
 short int gbFrecMixer_now[4]={0,0,0,0};

 int gb_ay8912_reg_select=0;
 int gb_ay8912_reg_value=0;
 int gb_ay8912_A_frec_fine=0;
 int gb_ay8912_B_frec_fine=0;
 int gb_ay8912_C_frec_fine=0;
 int gb_ay8912_A_frec_course=0;
 int gb_ay8912_B_frec_course=0;
 int gb_ay8912_C_frec_course=0;
 int gb_ay8912_noise_pitch=0;
 int gb_ay8912_A_frec=0;
 int gb_ay8912_B_frec=0;
 int gb_ay8912_C_frec=0;
 int gb_ay8912_A_vol=0;
 int gb_ay8912_B_vol=0;
 int gb_ay8912_C_vol=0;
 int gb_ay8912_mixer=0; 
 unsigned char gbShiftLeftVolumen=0; //Maximo volumen shift left 2
 unsigned char gb_mute_sound=0; 
 unsigned char gb_silence_all_channels=0;
#endif

#ifdef use_lib_resample_speaker
 unsigned int gbTimeIni=0;
 unsigned int gbTimeNow=0;
#endif 

#ifdef use_lib_mouse_kempston
 //short int gb_sdl_mouse_x;
 //short int gb_sdl_mouse_y;
 //short int gb_sdl_mouse_x_before;
 //short int gb_sdl_mouse_y_before; 
 //unsigned char gb_auxMouseBtn;
 //unsigned char gb_auxMouseBtn_before;
 int gb_z80_mouse_x=0;
 int gb_z80_mouse_y=0;
 int gb_z80_mouseBtn=0;
 unsigned char gb_mouse_init_error=0;
 unsigned char gb_mouse_key_btn_left=1;
 unsigned char gb_mouse_key_btn_right=1;
 unsigned char gb_mouse_key_btn_middle=1;
 unsigned char gb_mouse_key_left=1;
 unsigned char gb_mouse_key_right=1;
 unsigned char gb_mouse_key_up=1;
 unsigned char gb_mouse_key_down=1;  
 #ifdef use_lib_mouse_kempston_lefthanded
  unsigned char gb_force_left_handed=1;
 #else
  unsigned char gb_force_left_handed=0; 
 #endif
 //mouse fabgl::PS2Controller PS2Controller;
 unsigned char gb_mouse_ps2clk;
 unsigned char gb_mouse_ps2data;
 unsigned char gb_mouse_invert_deltax=0;
 unsigned char gb_mouse_invert_deltay=0;
 unsigned char gb_mouse_inc=1;
#endif

//#ifdef use_lib_mouse_kempston
// //fabgl::Mouse gb_mouse;
// fabgl::PS2Controller PS2Controller;
//#endif 

#ifndef use_lib_vga_thread
 unsigned long gbTimeVideoIni=0;
 unsigned long gbTimeVideoNow=0;
#endif 

#ifdef use_lib_lookup_ram
 static unsigned char gb_lookup_calcY[200]={
#else
 //Son 6400, quedan en 200 se repiten 32 veces
 const unsigned char gb_lookup_calcY[200]={
#endif     
  0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x01,0x09,0x11,0x19,0x21,0x29,0x31,0x39,
  0x02,0x0A,0x12,0x1A,0x22,0x2A,0x32,0x3A,0x03,0x0B,0x13,0x1B,0x23,0x2B,0x33,0x3B,
  0x04,0x0C,0x14,0x1C,0x24,0x2C,0x34,0x3C,0x05,0x0D,0x15,0x1D,0x25,0x2D,0x35,0x3D,
  0x06,0x0E,0x16,0x1E,0x26,0x2E,0x36,0x3E,0x07,0x0F,0x17,0x1F,0x27,0x2F,0x37,0x3F,
  0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x41,0x49,0x51,0x59,0x61,0x69,0x71,0x79,
  0x42,0x4A,0x52,0x5A,0x62,0x6A,0x72,0x7A,0x43,0x4B,0x53,0x5B,0x63,0x6B,0x73,0x7B,
  0x44,0x4C,0x54,0x5C,0x64,0x6C,0x74,0x7C,0x45,0x4D,0x55,0x5D,0x65,0x6D,0x75,0x7D,
  0x46,0x4E,0x56,0x5E,0x66,0x6E,0x76,0x7E,0x47,0x4F,0x57,0x5F,0x67,0x6F,0x77,0x7F,
  0x80,0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0x81,0x89,0x91,0x99,0xA1,0xA9,0xB1,0xB9,
  0x82,0x8A,0x92,0x9A,0xA2,0xAA,0xB2,0xBA,0x83,0x8B,0x93,0x9B,0xA3,0xAB,0xB3,0xBB,
  0x84,0x8C,0x94,0x9C,0xA4,0xAC,0xB4,0xBC,0x85,0x8D,0x95,0x9D,0xA5,0xAD,0xB5,0xBD,
  0x86,0x8E,0x96,0x9E,0xA6,0xAE,0xB6,0xBE,0x87,0x8F,0x97,0x9F,0xA7,0xAF,0xB7,0xBF,
  0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,0xF8
 };



// EXTERN VARS
//extern unsigned char gb_cfg_arch_is48K;
// Globals
unsigned char gb_cfg_arch_is48K = 1; //Modo 48k
#ifdef use_lib_core_linkefong
 extern CONTEXT _zxContext;
 extern Z80_STATE _zxCpu;
#endif 
extern int _total;
extern int _next_total;
extern unsigned char gb_cache_zxcolor[8];
//static unsigned char gb_local_cache_zxcolor[8]=
//{//cache de colores optimizado
// BLACK, BLUE, RED, MAGENTA, GREEN, CYAN, YELLOW, WHITE
//};


//JJ void load_rom(String, String);
//JJ void load_ram(String);
//void load_ram2flash(unsigned char id);
//JJ void SetMode48K(void);
//JJ void SetMode128K(void);

volatile unsigned char gb_sdl_blit=0;
//static unsigned long gb_currentTime=0;
static unsigned int gb_currentTime=0;

#ifdef use_lib_stats_time_unified
 #ifdef use_lib_core_linkefong 
  static unsigned int gb_ini_cpu_micros_unified;
  static unsigned int gb_fin_cpu_micros_unified;
 #endif 
 //static unsigned long gb_fps_time_ini_unified= 0;
 //static unsigned long gb_fps_unified= 0;
 //static unsigned long gb_fps_ini_unified= 0;
 //static unsigned long gb_stats_time_cur_unified= 0;
 static unsigned int gb_fps_time_ini_unified= 0;
 static unsigned int gb_fps_unified= 0;
 static unsigned int gb_fps_ini_unified= 0;
 static unsigned int gb_stats_time_cur_unified= 0; 
 static unsigned int gb_stats_time_min_unified= 500000;
 static unsigned int gb_stats_time_max_unified= 0;
 static unsigned int gb_stats_video_cur_unified= 0;
 static unsigned int gb_stats_video_min_unified= 500000;
 static unsigned int gb_stats_video_max_unified= 0;
#endif

#ifdef use_lib_sound_ay8912
 static unsigned long gb_sdl_time_sound_before=0;
#endif 
static unsigned int gb_keyboardTime;
#ifdef use_lib_mouse_kempston
 static unsigned int gb_mouseTime=0;
#endif 
static unsigned int gb_time_ini_espera;
unsigned char gb_run_emulacion = 1; //Ejecuta la emulacion
unsigned char gb_current_ms_poll_sound = gb_ms_sound-1;
unsigned char gb_current_ms_poll_keyboard = gb_ms_keyboard-1;
unsigned char gb_current_ms_poll_mouse = gb_ms_mouse-1;
//unsigned char gb_current_frame_crt_skip= gb_frame_crt_skip; //No salta frames
unsigned char gb_current_delay_emulate_ms= gb_delay_emulate_ms;
//unsigned char gb_current_delay_emulate_div_microsec= gb_delay_emulate_div_microsec;


//volatile byte keymap[256];
//volatile byte oldKeymap[256];

volatile unsigned char gbFrameSkipVideoCurrentCont=0;

// EXTERN METHODS
//void setup_cpuspeed();

//#ifdef use_lib_core_linkefong //No lo necesito aqui
// static uint8_t * gb_local_cache_rom[4] = { //No lo necesito aqui
//  rom0,rom1,rom2,rom3 //No lo necesito aqui
// }; //No lo necesito aqui
//
// static uint8_t * gb_local_cache_ram[8] = //No lo necesito aqui
// { //No lo necesito aqui
//  ram0,ram1,ram2,ram3,ram4,ram5,ram6,ram7 //No lo necesito aqui
// }; //No lo necesito aqui
//#endif //No lo necesito aqui

/*uint8_t local_fast_readbyte(uint16_t addr)
{
  switch (addr) 
  {
    case 0x0000 ... 0x3fff:
        return (gb_local_cache_rom[rom_in_use][addr]);
        break;
    case 0x4000 ... 0x7fff:
        return ram5[addr - 0x4000];
        break;
    case 0x8000 ... 0xbfff:
        return ram2[addr - 0x8000];
        break;
    case 0xc000 ... 0xffff:
        return (gb_local_cache_ram[bank_latch][(addr- 0xc000)]);        
        // Serial.printf("Address: %x Returned address %x  Bank: %x\n",addr,addr-0xc000,bank_latch);
        break;
  }    
}*/

#ifdef use_lib_vga_thread
 #ifdef use_lib_core_linkefong
  void videoTask(void *unused); 
 #else 
  #ifdef use_lib_core_jsanchezv
   void videoTask_jsanchezv(void *unused);
  #endif 
 #endif 
#endif 

//void swap_flash(word *a, word *b);
inline void swap_flash(unsigned char *a, unsigned char *b);

void PrepareColorsUltraFastVGA(void);

#ifdef use_lib_keyboard_uart
 unsigned int gb_curTime_keyboard_before_uart;
 unsigned int gb_curTime_keyboard_uart;
 void do_keyboard_uart(void);
#endif 



// GLOBALS

// keyboard ports read from PS2 keyboard
unsigned char z80ports_in[128]; //Comun para Lin Ke-Fong y JLS

// keyboard ports read from Wiimote
//JJ volatile byte z80ports_wiin[128];

volatile unsigned char borderTemp = 7;
volatile unsigned char flashing = 0;
volatile boolean xULAStop = false;
volatile boolean xULAStopped = false;
volatile unsigned char tick;
//const int SAMPLING_RATE = 44100;
//const int BUFFER_SIZE = 2000;

int halfsec, sp_int_ctr, evenframe, updateframe;

#ifdef use_lib_vga_thread
 QueueHandle_t vidQueue;
 TaskHandle_t videoTaskHandle;
 volatile bool videoTaskIsRunning = false;
 uint16_t *param;
#endif

// SETUP *************************************
//jjvga #ifdef use_lib_vga8colors
//jjvga  #ifdef use_lib_vga_low_memory
//jjvga   VGA3BitI vga;
//jjvga  #else
//jjvga   VGA3Bit vga;
//jjvga  #endif
//jjvga #else
//jjvga  #ifdef use_lib_vga64colors
//jjvga   #ifdef use_lib_vga_low_memory
//jjvga    VGA6BitI vga;
//jjvga   #else
//jjvga    //jjvga VGA6Bit vga;
//jjvga   #endif  
//jjvga  #endif
//jjvga #endif

//JJ #ifdef COLOR_14B
//JJ VGA14Bit vga;
//JJ #endif

#ifdef use_lib_sound_ay8912
 inline void sound_cycleFabgl(void);
 inline void jj_mixpsg(void);
#endif

#ifdef use_lib_mouse_kempston
 void PollMouse(void);
#endif

#ifdef use_lib_remap_keyboardpc 
 void do_keyboard_remap_pc(void);
#endif 

#ifdef use_lib_core_jsanchezv
 void loop_jsanchezv(void);
#endif

#ifdef use_lib_core_jsanchezv
 //int gb_z80_mouse_x;
 //int gb_z80_mouse_y;
 //int gb_z80_mouseBtn;

 unsigned char bank_latch_jsanchezv=0;
 unsigned char video_latch_jsanchezv = 0;
 unsigned char rom_latch_jsanchezv = 0;
 unsigned char paging_lock_jsanchezv = 0;
 unsigned char rom_in_use_jsanchezv=0;
 unsigned char sp3_rom_jsanchezv = 0;
 unsigned char sp3_mode_jsanchezv = 0;

 //unsigned char * rom0_jsanchezv;
 //unsigned char * rom1_jsanchezv;
 //unsigned char * rom2_jsanchezv;
 //unsigned char * rom3_jsanchezv; 
 unsigned char * ram1_jsanchezv;
 #ifdef use_lib_48k_iram_jsanchezv
  static unsigned char ram0_jsanchezv[0x4000];
  static unsigned char ram2_jsanchezv[0x4000];
  static unsigned char ram5_jsanchezv[0x4000];
 #else
  unsigned char * ram0_jsanchezv;
  unsigned char * ram2_jsanchezv;
  unsigned char * ram5_jsanchezv;
 #endif 
 unsigned char * ram3_jsanchezv;
 unsigned char * ram4_jsanchezv;
 
 unsigned char * ram6_jsanchezv;
 unsigned char * ram7_jsanchezv;

 unsigned char * z80Ports_jsanchezv;
 volatile unsigned char flashing_jsanchezv=0;
 int halfsec_jsanchezv=0;
 int sp_int_ctr_jsanchezv=0;
 unsigned char zx_data_jsanchezv=0;
 unsigned char borderTemp_jsanchezv=7;
 unsigned int cont_state_video_jsanchezv=0;
 unsigned char vsync_jsanchezv=0;
 unsigned char interruptPend_jsanchezv = 0;

 //int gbSDLVideoTimeBefore; //No se necesita

 //static unsigned char * gb_ram_z80Ram_jsanchezv;
 //static unsigned char * gb_ram_z80Ports_jsanchezv;
 //Medicion tiempos
 static unsigned int gb_time_state_jsanchez_ini=0;
 //static unsigned int gb_fps_jsanchezv=0;
 //static unsigned int gb_fps_time_ini_jsanchezv=0; //No se necesita
 //static unsigned int gb_fps_ini_jsanchezv=0; //No se necesita
 //static unsigned long antes_jsanchezv=0;
 //static unsigned long antes_jsanchezv=0;
 static unsigned int ahora_jsanchezv=0;
 static unsigned int antes_jsanchezv=0;
// #ifdef use_lib_stats_time_jsanchezv
//  static unsigned int gb_stats_time_cur_jsanchezv=0;
//  static unsigned int gb_stats_time_min_jsanchezv=500000;
//  static unsigned int gb_stats_time_max_jsanchezv=0;
// #endif
 //#ifdef use_lib_stats_video_jsanchezv 
 // static unsigned int gb_stats_video_cur_jsanchezv=0;
 // static unsigned int gb_stats_video_min_jsanchezv=500000;
 // static unsigned int gb_stats_video_max_jsanchezv=0;  
 //#endif 


 unsigned char gb_ptr_IdRomRam_jsanchezv[4]={
  0,5,2,0
 }; //El ultimo es bank_latch
 
 uint8_t * gb_local_cache_ram_jsanchezv[8] =
 {
  ram0_jsanchezv,ram1_jsanchezv,ram2_jsanchezv,ram3_jsanchezv,ram4_jsanchezv,ram5_jsanchezv,ram6_jsanchezv,ram7_jsanchezv 
 }; 

 uint8_t * gb_local_cache_rom_jsanchezv[4] = {
  //rom0,rom1,rom2,rom3
  rom0_jsanchezv,rom1_jsanchezv,rom2_jsanchezv,rom3_jsanchezv
 }; 

//************************************************
/*
void videoTaskNoThread_jsanchezv()
{
    //8300 microsegundos
    //7680 microsegundos quitando desplazamiento y skip frame
    //7500 micros 320x200 8 colores ultrafast

   #ifdef use_lib_stats_video_jsanchezv
    static unsigned long time_prev;    
   #endif 
    int byte_offset;
    unsigned char color_attrib, flash;
    //int pixel_map;//, bright;
    int zx_vidcalc, calc_y;    

    word zx_fore_color, zx_back_color, tmp_color;    

    unsigned char auxColor;
    unsigned char * gb_ptr_ram_1800_video;
    unsigned char * gb_ptr_ram_video;
    unsigned char bitpos;
    #ifdef use_lib_ultrafast_vga
     unsigned char ** ptrVGA;
     #define gbvgaMask8Colores 0x3F
     #define gbvgaBits8Colores 0x40
     ptrVGA = vga.backBuffer;
    #endif

    if (!video_latch_jsanchezv){
     gb_ptr_ram_1800_video = &ram5_jsanchezv[0x1800];
     gb_ptr_ram_video = ram5_jsanchezv;
    }
    else{
     gb_ptr_ram_1800_video = &ram7_jsanchezv[0x1800];
     gb_ptr_ram_video = ram7_jsanchezv;  
    }    

        #ifdef use_lib_stats_video_jsanchezv
         time_prev = micros();     
        #endif 
        for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
        {            
            #ifdef use_lib_ultrafast_vga
             #ifdef use_lib_vga_low_memory
              //Modo bajo y rapido
              #ifdef use_lib_vga8colors
               auxColor = (gb_cache_zxcolor[borderTemp_jsanchezv]); //8 colores seguro bajo
              #else               
               auxColor = (gb_cache_zxcolor[borderTemp_jsanchezv] & gbvgaMask8Colores)|gbvgaBits8Colores; //Modo bajo
              #endif 
             #else
              //Modo alto y rapido 
              //auxColor = (gb_cache_zxcolor[borderTemp_jsanchezv] & gbvgaMask8Colores)|gbvgaBits8Colores;
              auxColor = gb_cache_zxcolor[borderTemp_jsanchezv]; //Directo
             #endif 
            #else
             //Modo lento
             auxColor = (gb_cache_zxcolor[borderTemp_jsanchezv]);
            #endif
                
            if (vga_lin < 3 || vga_lin > 194)
            {
                #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)                 
                 for (int bor = 0; bor < 296; bor++)
                #else 
                 for (int bor = 32; bor < 328; bor++)                
                #endif
                {                    
                  #ifdef use_lib_ultrafast_vga
                   #ifdef use_lib_vga_low_memory
                    //Modo bajo y rapido
                    #ifdef use_lib_vga8colors
                     vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                    #else 
                     ptrVGA[vga_lin][bor] = auxColor; //Modo bajo
                    #endif 
                   #else
                    //Modo alto y rapido
                    ptrVGA[vga_lin][bor^2] = auxColor;
                   #endif
                  #else
                   //Modo lento                   
                   vga.dotFast(bor,vga_lin,auxColor);
                  #endif
                }                
            }
            else            
            {
                #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)                 
                 for (int bor = 0; bor < 20; bor++)
                #else
                 for (int bor = 32; bor < 52; bor++)
                #endif               
                {                    
                 #ifdef use_lib_ultrafast_vga
                  #ifdef use_lib_vga_low_memory
                   //Modo bajo y rapido
                   #ifdef use_lib_vga8colors
                    vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                    vga.dotFast((bor+276),vga_lin,auxColor);
                   #else 
                    ptrVGA[vga_lin][bor] = auxColor; //Modo bajo
                    ptrVGA[vga_lin][(bor+276)] = auxColor;
                   #endif
                  #else
                   //Modo alto y rapido
                   ptrVGA[vga_lin][bor^2] = auxColor;
                   ptrVGA[vga_lin][(bor+276)^2] = auxColor;
                  #endif
                 #else
                  //Modo lento
                  vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                  vga.dotFast((bor+276),vga_lin,auxColor);
                 #endif
                }
                byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
                for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
                {                    
                    //calc_y = calcY(byte_offset);                    
                    calc_y = gb_lookup_calcY[(byte_offset>>5)];
                    color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values
                    zx_vidcalc = (ff << 3);
                    bitpos = 0x80;
                    for (unsigned char i = 0; i < 8; i++) // foreach pixel within a byte
                    {
                        flash = (color_attrib & 0B10000000) >> 7;
                        zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                        zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
                        if (flash && flashing_jsanchezv)
                        {
                         swap_flash(&zx_fore_color, &zx_back_color);
                        }
                        #ifdef use_lib_ultrafast_vga
                         //auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores);
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color;
                        #else
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color : zx_back_color;
                        #endif

                        
                        #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                         //320x240
                         #ifdef use_lib_ultrafast_vga
                          #ifdef use_lib_vga_low_memory
                           //Modo bajo y rapido
                           #ifdef use_lib_vga8colors
                            vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                           #else 
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)] = auxColor;
                           #endif
                          #else
                           //Modo alto y rapido  
                           ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)^2] = auxColor;
                          #endif                            
                         #else
                          //Modo lento
                          vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                         #endif                         
                        #else
                         //360x200
                         #ifdef use_lib_ultrafast_vga
                          #ifdef use_lib_vga_low_memory
                           //Modo bajo y rapido
                           #ifdef use_lib_vga8colors
                            vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor); //8 colores seguro bajo
                           #else
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)] = auxColor;  //Modo bajo
                           #endif
                          #else
                           //Modo alto y rapido
                           ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                          #endif                           
                         #else
                          //Modo lento 
                          vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor);
                         #endif
                        #endif 

                        zx_vidcalc++;
                        bitpos = (bitpos>>1);
                    }
                    byte_offset++;
                }
            }
        }
        //gb_sdl_blit=1;
     #ifdef use_lib_stats_video_jsanchezv
      time_prev = micros()-time_prev;
      gb_stats_video_cur_jsanchezv = time_prev;
      if (time_prev< gb_stats_video_min_jsanchezv)
       gb_stats_video_min_jsanchezv= time_prev;
      if (time_prev> gb_stats_video_max_jsanchezv)
       gb_stats_video_max_jsanchezv= time_prev;
     #endif  
     #ifdef use_lib_log_serial
      //Serial.printf("Tiempo %d\n",time_prev);             
     #endif
}
*/

#ifdef use_lib_core_jsanchezv
 #ifdef use_lib_vga_thread
  void videoTask_jsanchezv(void *unused)
  {
   unsigned char paleta[2]; //0 backcolor 1 Forecolor
   unsigned int a0,a1,a2,a3,a32;

   #ifdef use_lib_stats_time_unified
    unsigned long time_prev;
   #endif
   int byte_offset;
   unsigned char color_attrib, flash;
   //int pixel_map;//, bright;
   int zx_vidcalc, calc_y;    

   //word zx_fore_color, zx_back_color, tmp_color;
   unsigned char zx_fore_color, zx_back_color;
   //, tmp_color; //No se usa

   unsigned char auxColor;    
   unsigned char * gb_ptr_ram_1800_video;
   unsigned char * gb_ptr_ram_video;
   //unsigned char bitpos;

   #ifdef use_lib_skip_frame
    volatile unsigned char skipFrame=0;
   #endif
   videoTaskIsRunning = true;
   uint16_t *param;

  while (1) 
  {        
   xQueuePeek(vidQueue, &param, portMAX_DELAY);
   if ((int)param == 1){
    break;
   }
   #ifdef use_lib_skip_frame
    if(
       (gbFrameSkipVideoCurrentCont < gbFrameSkipVideoMaxCont)
       && 
       (gbFrameSkipVideoMaxCont != 0)
      )
    {
     gbFrameSkipVideoCurrentCont++; 
     skipFrame = 1;
    }
    else
    { 
     gbFrameSkipVideoCurrentCont= 0;
     skipFrame = 0;
    }
   #endif


  #ifdef use_lib_skip_frame
  if ((skipFrame != 1) && (gb_draw_thread == 1))
  #else
  if (gb_draw_thread == 1)
  #endif
  {

   if (!video_latch_jsanchezv)
   {
    gb_ptr_ram_1800_video = &ram5_jsanchezv[0x1800];
    gb_ptr_ram_video = ram5_jsanchezv;
   }
   else
   {
    gb_ptr_ram_1800_video = &ram7_jsanchezv[0x1800];
    gb_ptr_ram_video = ram7_jsanchezv;  
   }    

   #ifdef use_lib_stats_time_unified
    time_prev = micros();
   #endif
   for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
   {     
    auxColor = gb_cache_zxcolor[borderTemp_jsanchezv];
    a0= auxColor;
    a32= a0 | (a0<<8) | (a0<<16) | (a0<<24);
    if (vga_lin < 3 || vga_lin > 194)
    {
     #ifdef use_lib_screen_offset
      if ((gb_screen_yIni+vga_lin)<0)
      {
       continue;
      }
     #endif

     #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)
      for (int bor = 0; bor < 74; bor++) //296 DIV 4 32 bits
     #else
      for (int bor = 8; bor < 82; bor++) //32 DIV 4   328 DIV 4 32 bits
     #endif   
     {                    
      //ptrVGA[vga_lin][bor^2] = auxColor;
      #ifdef use_lib_screen_offset
       gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
      #else
       gb_buffer_vga32[vga_lin][bor] = a32;
      #endif 
     }                
    }
    else            
    {
     #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)
      for (int bor = 0; bor < 5; bor++) //20 DIV 4 32 bits
     #else
      for (int bor = 8; bor < 13; bor++) //32 DIV 4   52 DIV 4 32 bits
     #endif   
     {                    
       //ptrVGA[vga_lin][bor^2] = auxColor;
       //ptrVGA[vga_lin][(bor+276)^2] = auxColor;       
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
        gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+69+gb_screen_xIni] = a32;       
       #else
        gb_buffer_vga32[vga_lin][bor] = a32;
        gb_buffer_vga32[vga_lin][bor+69] = a32;
       #endif 
     }
     byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
          
     for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
     {                    
      //calc_y = calcY(byte_offset);                    
      calc_y = gb_lookup_calcY[(byte_offset>>5)];
      color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
      //zx_vidcalc = (ff << 3); //DIV 4 32 bits
      //zx_vidcalc= (ff<<3) >> 2;  //Ya no lo necesito

      //BEGIN El color lo saco fuera 8 pixels
      flash = (color_attrib & 0B10000000) >> 7;
      #ifdef use_lib_use_bright
       if (((color_attrib & 0B01000000) >> 6) == 0x01)
       {
        zx_fore_color = gb_cache_zxcolor_bright[(color_attrib & 0B00000111)]; //con brillo
        zx_back_color = gb_cache_zxcolor_bright[((color_attrib & 0B00111000) >> 3)];//con brillo                             
       }
       else
       {
        zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //sin brillo
        zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//sin brillo
       }                    
      #else
       zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
       zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
      #endif 
      if (flash && flashing_jsanchezv)
      {
       swap_flash(&zx_fore_color, &zx_back_color);
      }
                    
      paleta[0]= zx_back_color;
      paleta[1]= zx_fore_color;
      
      zx_vidcalc= (ff<<1); //Lo saco fuera 32 bits

      a0= paleta[((gb_ptr_ram_video[byte_offset] >>7) & 0x01)];
      a1= paleta[((gb_ptr_ram_video[byte_offset] >>6) & 0x01)];
      a2= paleta[((gb_ptr_ram_video[byte_offset] >>5) & 0x01)];
      a3= paleta[((gb_ptr_ram_video[byte_offset] >>4) & 0x01)];
                     
      a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
      #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)    
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+5+gb_screen_xIni] = a32; //20 DIV 4 = 5
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+5] = a32; //20 DIV 4 = 5
       #endif 
      #else
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+13+gb_screen_xIni] = a32; //52 DIV 4 = 13
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+13] = a32; //52 DIV 4 = 13
       #endif 
      #endif 

      a0= paleta[((gb_ptr_ram_video[byte_offset] >>3) & 0x01)];
      a1= paleta[((gb_ptr_ram_video[byte_offset] >>2) & 0x01)];
      a2= paleta[((gb_ptr_ram_video[byte_offset] >>1) & 0x01)];
      a3= paleta[((gb_ptr_ram_video[byte_offset]) & 0x01)];
                     
      a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
      #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+6+gb_screen_xIni] = a32; //20 DIV 4 = 5
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+6] = a32; //20 DIV 4 = 5
       #endif 
      #else
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+14+gb_screen_xIni] = a32; //52 DIV 4 = 13
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+14] = a32; //52 DIV 4 = 13
       #endif 
      #endif 
                

      byte_offset++;
     }//fin for
    }
   }
   //gb_sdl_blit=1;
    
   #ifdef use_lib_stats_time_unified
    time_prev = micros()-time_prev;
    gb_stats_video_cur_unified = time_prev;
    if (time_prev< gb_stats_video_min_unified){
     gb_stats_video_min_unified= time_prev;
    }
    if (time_prev> gb_stats_video_max_unified){
     gb_stats_video_max_unified= time_prev;
    }
   #endif
  }//fin if skipframe

  xQueueReceive(vidQueue, &param, portMAX_DELAY);
  videoTaskIsRunning = false;
  }
  videoTaskIsRunning = false;
  vTaskDelete(NULL);

  while (1)
  {
  }    

  } 
 #else
  void videoTaskNoThread_jsanchezv()
  {

//2272 micros con array paleta
 //2561 microsegundos

 //8300 microsegundos
 //7680 microsegundos quitando desplazamiento y skip frame
 //unsigned int cont_a=0;
 unsigned char paleta[2]; //0 backcolor 1 Forecolor
 unsigned int a0,a1,a2,a3,a32;

 #ifdef use_lib_stats_time_unified
  unsigned long time_prev;
 #endif

 #ifdef use_lib_skip_frame
  unsigned char skipFrame=0;
 #endif
 int byte_offset;
 unsigned char color_attrib, flash;
 //int pixel_map;//, bright;
 int zx_vidcalc, calc_y;    

 //word zx_fore_color, zx_back_color, tmp_color;
 unsigned char zx_fore_color, zx_back_color, tmp_color;

 unsigned char auxColor;    
 unsigned char * gb_ptr_ram_1800_video;
 unsigned char * gb_ptr_ram_video;
 //unsigned char bitpos;    
 #ifdef use_lib_skip_frame
  if(
     (gbFrameSkipVideoCurrentCont < gbFrameSkipVideoMaxCont)
     && 
     (gbFrameSkipVideoMaxCont != 0)
    )
  {
   gbFrameSkipVideoCurrentCont++; 
   skipFrame = 1;
  }
  else
  { 
   gbFrameSkipVideoCurrentCont= 0;
   skipFrame = 0;
  }
 #endif


#ifdef use_lib_skip_frame
if (skipFrame == 1){
 return;
}
#endif


 if (!video_latch_jsanchezv)
 {
  gb_ptr_ram_1800_video = &ram5_jsanchezv[0x1800];
  gb_ptr_ram_video = ram5_jsanchezv;
 }
 else
 {
  gb_ptr_ram_1800_video = &ram7_jsanchezv[0x1800];
  gb_ptr_ram_video = ram7_jsanchezv;  
 }    

 #ifdef use_lib_stats_time_unified
  time_prev = micros();
 #endif
 for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
 {     
  auxColor = gb_cache_zxcolor[borderTemp_jsanchezv];
  a0= auxColor;
  a32= a0 | (a0<<8) | (a0<<16) | (a0<<24);
  if (vga_lin < 3 || vga_lin > 194)
  {
   #ifdef use_lib_screen_offset
    if ((gb_screen_yIni+vga_lin)<0)
    {
     continue;
    }
   #endif

   #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)
    for (int bor = 0; bor < 74; bor++) //296 DIV 4 32 bits
   #else
    for (int bor = 8; bor < 82; bor++) //32 DIV 4   328 DIV 4 32 bits
   #endif   
   {                    
    //ptrVGA[vga_lin][bor^2] = auxColor;
    #ifdef use_lib_screen_offset
     gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
    #else
     gb_buffer_vga32[vga_lin][bor] = a32;
    #endif
   }                
  }
  else            
  {
   #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)
    for (int bor = 0; bor < 5; bor++) //20 DIV 4 32 bits
   #else
    for (int bor = 8; bor < 13; bor++) //32 DIV 4   52 DIV 4 32 bits
   #endif   
   {                    
     //ptrVGA[vga_lin][bor^2] = auxColor;
     //ptrVGA[vga_lin][(bor+276)^2] = auxColor;
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
      gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+69+gb_screen_xIni] = a32;     
     #else
      gb_buffer_vga32[vga_lin][bor] = a32;
      gb_buffer_vga32[vga_lin][bor+69] = a32;
     #endif 
   }
   byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
          
   for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
   {                    
    //calc_y = calcY(byte_offset);                    
    calc_y = gb_lookup_calcY[(byte_offset>>5)];
    color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
    //zx_vidcalc = (ff << 3); //DIV 4 32 bits
    //zx_vidcalc= (ff<<3) >> 2;  //Ya no lo necesito

    //BEGIN El color lo saco fuera 8 pixels
    flash = (color_attrib & 0B10000000) >> 7;
    #ifdef use_lib_use_bright
     if (((color_attrib & 0B01000000) >> 6) == 0x01)
     {
      zx_fore_color = gb_cache_zxcolor_bright[(color_attrib & 0B00000111)]; //con brillo
      zx_back_color = gb_cache_zxcolor_bright[((color_attrib & 0B00111000) >> 3)];//con brillo                             
     }
     else
     {
      zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //sin brillo
      zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//sin brillo
     }                    
    #else
     zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
     zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
    #endif 
    if (flash && flashing_jsanchezv)
    {
     swap_flash(&zx_fore_color, &zx_back_color);
    }
                    
    paleta[0]= zx_back_color;
    paleta[1]= zx_fore_color;
      
    zx_vidcalc= (ff<<1); //Lo saco fuera 32 bits

    a0= paleta[((gb_ptr_ram_video[byte_offset] >>7) & 0x01)];
    a1= paleta[((gb_ptr_ram_video[byte_offset] >>6) & 0x01)];
    a2= paleta[((gb_ptr_ram_video[byte_offset] >>5) & 0x01)];
    a3= paleta[((gb_ptr_ram_video[byte_offset] >>4) & 0x01)];
                     
    a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
    #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)    
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+5+gb_screen_xIni] = a32; //20 DIV 4 = 5
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+5] = a32; //20 DIV 4 = 5
     #endif 
    #else
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+13+gb_screen_xIni] = a32; //52 DIV 4 = 13
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+13] = a32; //52 DIV 4 = 13
     #endif 
    #endif 

    a0= paleta[((gb_ptr_ram_video[byte_offset] >>3) & 0x01)];
    a1= paleta[((gb_ptr_ram_video[byte_offset] >>2) & 0x01)];
    a2= paleta[((gb_ptr_ram_video[byte_offset] >>1) & 0x01)];
    a3= paleta[((gb_ptr_ram_video[byte_offset]) & 0x01)];
                     
    a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
    #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+6+gb_screen_xIni] = a32; //20 DIV 4 = 5
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+6] = a32; //20 DIV 4 = 5
     #endif 
    #else
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+14+gb_screen_xIni] = a32; //52 DIV 4 = 13
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+14] = a32; //52 DIV 4 = 13
     #endif 
    #endif 
                

    byte_offset++;
   }//fin for
  }
 }
 //gb_sdl_blit=1;


 #ifdef use_lib_stats_time_unified
  time_prev = micros()-time_prev;
  gb_stats_video_cur_unified = time_prev;
  if (time_prev< gb_stats_video_min_unified){
   gb_stats_video_min_unified= time_prev;
  }
  if (time_prev> gb_stats_video_max_unified){
   gb_stats_video_max_unified= time_prev;
  }
 #endif


 //time_prev = micros()-time_prev;
 //#ifdef use_lib_log_serial
 // Serial.printf("Tiempo %d\n",time_prev);             
 //#endif 

  }
 #endif 
#endif


 void keyboard_do_jsanchezv()
 {
    //byte kempston = 0; //No se usa

  #ifdef use_lib_remap_keyboardpc
   //Cursores menu en 128k
   if (keymap[KEY_CURSOR_UP] == 0){ 
    keymap[0x12]= keymap[0x3d] = 0; //shift+7
   }
   else
   {
    if (keymap[KEY_CURSOR_DOWN] == 0) { keymap[0x12]= keymap[0x36]= 0; }
    //else {keymap[0x12]= keymap[0x36]= 1;}//shift+6   
   }
  #endif 

    //gb_show_osd_main_menu = !keymap[KEY_F1];
    //if (!keymap[KEY_F1])
    //{
    // gb_show_osd_main_menu = 1;
    // do_tinyOSD();
    //}

    bitWrite(z80Ports_jsanchezv[0], 0, keymap[0x12]);
    bitWrite(z80Ports_jsanchezv[0], 1, keymap[0x1a]);
    bitWrite(z80Ports_jsanchezv[0], 2, keymap[0x22]);
    bitWrite(z80Ports_jsanchezv[0], 3, keymap[0x21]);
    bitWrite(z80Ports_jsanchezv[0], 4, keymap[0x2a]);

    bitWrite(z80Ports_jsanchezv[1], 0, keymap[0x1c]);
    bitWrite(z80Ports_jsanchezv[1], 1, keymap[0x1b]);
    bitWrite(z80Ports_jsanchezv[1], 2, keymap[0x23]);
    bitWrite(z80Ports_jsanchezv[1], 3, keymap[0x2b]);
    bitWrite(z80Ports_jsanchezv[1], 4, keymap[0x34]);

    bitWrite(z80Ports_jsanchezv[2], 0, keymap[0x15]);
    bitWrite(z80Ports_jsanchezv[2], 1, keymap[0x1d]);
    bitWrite(z80Ports_jsanchezv[2], 2, keymap[0x24]);
    bitWrite(z80Ports_jsanchezv[2], 3, keymap[0x2d]);
    bitWrite(z80Ports_jsanchezv[2], 4, keymap[0x2c]);

    bitWrite(z80Ports_jsanchezv[3], 0, keymap[0x16]);
    bitWrite(z80Ports_jsanchezv[3], 1, keymap[0x1e]);
    bitWrite(z80Ports_jsanchezv[3], 2, keymap[0x26]);
    bitWrite(z80Ports_jsanchezv[3], 3, keymap[0x25]);
    bitWrite(z80Ports_jsanchezv[3], 4, keymap[0x2e]);

    bitWrite(z80Ports_jsanchezv[4], 0, keymap[0x45]);
    bitWrite(z80Ports_jsanchezv[4], 1, keymap[0x46]);
    bitWrite(z80Ports_jsanchezv[4], 2, keymap[0x3e]);
    bitWrite(z80Ports_jsanchezv[4], 3, keymap[0x3d]);
    bitWrite(z80Ports_jsanchezv[4], 4, keymap[0x36]);

    bitWrite(z80Ports_jsanchezv[5], 0, keymap[0x4d]);
    bitWrite(z80Ports_jsanchezv[5], 1, keymap[0x44]);
    bitWrite(z80Ports_jsanchezv[5], 2, keymap[0x43]);
    bitWrite(z80Ports_jsanchezv[5], 3, keymap[0x3c]);
    bitWrite(z80Ports_jsanchezv[5], 4, keymap[0x35]);

    bitWrite(z80Ports_jsanchezv[6], 0, keymap[0x5a]);
    bitWrite(z80Ports_jsanchezv[6], 1, keymap[0x4b]);
    bitWrite(z80Ports_jsanchezv[6], 2, keymap[0x42]);
    bitWrite(z80Ports_jsanchezv[6], 3, keymap[0x3b]);
    bitWrite(z80Ports_jsanchezv[6], 4, keymap[0x33]);

    bitWrite(z80Ports_jsanchezv[7], 0, keymap[0x29]);
    bitWrite(z80Ports_jsanchezv[7], 1, keymap[0x14]);
    bitWrite(z80Ports_jsanchezv[7], 2, keymap[0x3a]);
    bitWrite(z80Ports_jsanchezv[7], 3, keymap[0x31]);
    bitWrite(z80Ports_jsanchezv[7], 4, keymap[0x32]);

    //Serial.printf("Teclado %d %d\n",keymap[KEY_CURSOR_UP],keymap[KEY_CURSOR_DOWN]);
    // Kempston joystick
    z80Ports_jsanchezv[0x1f] = 0;
    bitWrite(z80Ports_jsanchezv[0x1f], 0, !keymap[KEY_CURSOR_RIGHT]);
    bitWrite(z80Ports_jsanchezv[0x1f], 1, !keymap[KEY_CURSOR_LEFT]);
    bitWrite(z80Ports_jsanchezv[0x1f], 2, !keymap[KEY_CURSOR_DOWN]);
    bitWrite(z80Ports_jsanchezv[0x1f], 3, !keymap[KEY_CURSOR_UP]);
    bitWrite(z80Ports_jsanchezv[0x1f], 4, !keymap[KEY_ALT_GR]);          

 #ifdef use_lib_remap_keyboardpc
  if (keymap[KEY_CURSOR_UP] == 0){ 
   keymap[0x12]= keymap[0x3d] = 1; //shift+7
  }
  else
  {
   if (keymap[KEY_CURSOR_DOWN] == 0) { keymap[0x12]= keymap[0x36]= 1; }
   //else {keymap[0x12]= keymap[0x36]= 1;}//shift+6   
  }
 #endif    
 }

 //***************************************************              
 void AsignarRom_jsanchezv()
 {
  #ifdef use_lib_rom0_inRAM_jsanchezv     
   memcpy(rom0_inRAM_jsanchezv,gb_list_roms_48k_data[0],0x4000);
   rom0_jsanchezv= (uint8_t *)rom0_inRAM_jsanchezv;
  #else 
   rom0_jsanchezv= (uint8_t *)gb_list_roms_48k_data[0];//rom 48K
  #endif 
  rom1_jsanchezv= (uint8_t *)gb_list_roms_48k_data[0];
  rom2_jsanchezv= (uint8_t *)gb_list_roms_48k_data[0];
  rom3_jsanchezv= (uint8_t *)gb_list_roms_48k_data[0];
 }


// void PreparaVGAColoresMascara()
// {
//    for (unsigned char i=0;i<8;i++)
//    {
//      gb_cache_zxcolor[i]= (gb_cache_zxcolor[i] & vga.RGBAXMask)|vga.SBits;
//    }  
// }

 #ifndef use_lib_wifi 
 void Z80sim::load_ram2Flash128_jsanchezv(unsigned char id)
 {
  int contBuffer=0; 
  byte sp_h, sp_l;
  uint16_t retaddr;
  bool auxIFF2=false;
  //unsigned char auxSwap; //No se usa

  #ifdef use_lib_sound_ay8912
   ResetSound();
  #endif
  if (id >= max_list_sna_128)
   return;
  zx_reset_jsanchezv();
  
  memset(ram0_jsanchezv,0,0x4000);
  memset(ram1_jsanchezv,0,0x4000);
  memset(ram2_jsanchezv,0,0x4000);
  memset(ram3_jsanchezv,0,0x4000);
  memset(ram4_jsanchezv,0,0x4000);
  memset(ram5_jsanchezv,0,0x4000);
  memset(ram6_jsanchezv,0,0x4000);
  memset(ram7_jsanchezv,0,0x4000);  
 
    // Read in the registers    
    cpu.setRegI(gb_list_sna_128k_data[id][0]); //lhandle.read();
    cpu.setRegLx(gb_list_sna_128k_data[id][1]);//lhandle.read();
    cpu.setRegHx(gb_list_sna_128k_data[id][2]);//lhandle.read();
    cpu.setRegEx(gb_list_sna_128k_data[id][3]);//lhandle.read();
    cpu.setRegDx(gb_list_sna_128k_data[id][4]);//lhandle.read();
    cpu.setRegCx(gb_list_sna_128k_data[id][5]);//lhandle.read();
    cpu.setRegBx(gb_list_sna_128k_data[id][6]);//lhandle.read();
    cpu.setRegFx(gb_list_sna_128k_data[id][7]);//lhandle.read();
    cpu.setRegAx(gb_list_sna_128k_data[id][8]);//lhandle.read();

    //_zxCpu.alternates[Z80_HL] = _zxCpu.registers.word[Z80_HL];
    //_zxCpu.alternates[Z80_DE] = _zxCpu.registers.word[Z80_DE];
    //_zxCpu.alternates[Z80_BC] = _zxCpu.registers.word[Z80_BC];
    //_zxCpu.alternates[Z80_AF] = _zxCpu.registers.word[Z80_AF];

    cpu.setRegL(gb_list_sna_128k_data[id][9]);//lhandle.read();
    cpu.setRegH(gb_list_sna_128k_data[id][10]);//lhandle.read();
    cpu.setRegE(gb_list_sna_128k_data[id][11]);//lhandle.read();
    cpu.setRegD(gb_list_sna_128k_data[id][12]);//lhandle.read();
    cpu.setRegC(gb_list_sna_128k_data[id][13]);//lhandle.read();
    cpu.setRegB(gb_list_sna_128k_data[id][14]);//lhandle.read();
    sp_l= gb_list_sna_128k_data[id][15];
    sp_h= gb_list_sna_128k_data[id][16];
    cpu.setRegIY(sp_l + sp_h * 0x100);
    sp_l= gb_list_sna_128k_data[id][17];
    sp_h= gb_list_sna_128k_data[id][18];
    cpu.setRegIX(sp_l + sp_h * 0x100);
    
    byte inter = gb_list_sna_128k_data[id][19];//lhandle.read();
    auxIFF2 = (inter & 0x04) ? true : false;
    cpu.setIFF2(auxIFF2);

    cpu.setRegR(gb_list_sna_128k_data[id][20]);
    cpu.setFlags(gb_list_sna_128k_data[id][21]);
    cpu.setRegA(gb_list_sna_128k_data[id][22]);
    sp_l = gb_list_sna_128k_data[id][23];//lhandle.read();
    sp_h = gb_list_sna_128k_data[id][24];//lhandle.read();
    cpu.setRegSP(sp_l + sp_h * 0x100);        
    
    switch (gb_list_sna_128k_data[id][25])
    {
     case 0: cpu.setIM(cpu.IM0); break;
     case 1: cpu.setIM(cpu.IM1); break;
     case 2: cpu.setIM(cpu.IM2); break;
	}

    byte bordercol = gb_list_sna_128k_data[id][26];//lhandle.read();
    borderTemp_jsanchezv = bordercol;    
    cpu.setIFF1(auxIFF2);     
 
    contBuffer = 27;
    //uint16_t buf_p = 0x4000;
        
    memcpy(ram5_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000);
    contBuffer+= 0x4000;
    memcpy(ram2_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); //0x4000 + 17d
    contBuffer+= 0x8000; //Saltamos el banco cacheado 0x4000+0x4000
    
    byte retaddr_l = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read();
    byte retaddr_h = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read();
    retaddr = retaddr_l + retaddr_h * 0x100;
    byte tmp_port = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read();
    //unsigned char auxBank= (tmp_port&0x07); //No se usa
    //printf("port 0x7ffd %x banco:%d\n",tmp_port,auxBank);
    //fflush(stdout);
    
    //byte tr_dos = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read(); //No se usa
    contBuffer++; //Incrementamos lectura aunque no usemos tr_dos
    byte tmp_latch = tmp_port & 0x7;
    bank_latch_jsanchezv = tmp_latch; gb_ptr_IdRomRam_jsanchezv[3] = bank_latch_jsanchezv;
    #ifdef use_optimice_writebyte
     gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[bank_latch_jsanchezv];
     gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[bank_latch_jsanchezv];
    #endif    
    
    contBuffer= 0x801B; //27+0x4000+0x4000
    switch (bank_latch_jsanchezv)
    {
     case 0: memcpy(ram0_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 1: memcpy(ram1_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 2: memcpy(ram2_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 3: memcpy(ram3_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 4: memcpy(ram4_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 5: memcpy(ram5_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 6: memcpy(ram6_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 7: memcpy(ram7_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
    }
    contBuffer+= 0x4004; //0x4000+4    
    //131103 bytes de SNA 128                
    //Abadia banco 4 latch, luego 0, 1, 3, 6 y 7
    for (unsigned char i=0;i<8;i++)
    {
     if (i != 2 && i != 5 && i != bank_latch_jsanchezv)
     {
      switch (i)
      {
       case 0: memcpy(ram0_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 1: memcpy(ram1_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 3: memcpy(ram3_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 4: memcpy(ram4_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 6: memcpy(ram6_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 7: memcpy(ram7_jsanchezv,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;                            
      }
      contBuffer+= 0x4000;           
     }
    }

    video_latch_jsanchezv = bitRead(tmp_port, 3);
    rom_latch_jsanchezv = bitRead(tmp_port, 4);
    paging_lock_jsanchezv = bitRead(tmp_port, 5);
    bank_latch_jsanchezv = tmp_latch; gb_ptr_IdRomRam_jsanchezv[3] = tmp_latch;
    rom_in_use_jsanchezv = rom_latch_jsanchezv; 
    #ifdef use_optimice_writebyte
     gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[rom_in_use_jsanchezv];
     gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[bank_latch_jsanchezv];
     gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[bank_latch_jsanchezv];
    #endif    
        
    //_zxCpu.pc = retaddr;
    cpu.setRegPC(retaddr);
 }
 #endif

 #ifdef use_lib_sna_uart
  //Lee SNA desde UART
  void Z80sim::load_ram2FlashFromUART_jsanchezv(unsigned char isSNA48K)
  {
   char cadout[32];    
   if (isSNA48K != 1)
   {
    //Pendiente load_ram2Flash128FromUART_jsanchezv();
    return;
   }

   Serial.setTimeout(0);
   Serial.flush();
   SDLprintText("WAIT UART",50,20,0,7);

   int contBuffer=0; 
   //uint16_t size_read; //No se usa
   byte sp_h, sp_l;
   uint16_t retaddr; 
	 bool auxIFF2=false;
	 //unsigned char auxSwap;  //No se usa
    
    #ifdef use_lib_only_48k
     memset(ram0_jsanchezv,0,0x4000);
     memset(ram2_jsanchezv,0,0x4000);
     memset(ram5_jsanchezv,0,0x4000);
    #else
     memset(ram0_jsanchezv,0,0x4000);
     memset(ram1_jsanchezv,0,0x4000);
     memset(ram2_jsanchezv,0,0x4000);
     memset(ram3_jsanchezv,0,0x4000);
     memset(ram4_jsanchezv,0,0x4000);
     memset(ram5_jsanchezv,0,0x4000);
     memset(ram6_jsanchezv,0,0x4000);
     memset(ram7_jsanchezv,0,0x4000);
    #endif 
               
    cpu.reset(); //zx_reset();        
    //Hago lo mismo que zx_reset
    zx_reset_jsanchezv();    
    //Fin lo mismo zx_reset

    if (gb_cfg_arch_is48K == 1)
    {
     rom_latch_jsanchezv = 0;
     rom_in_use_jsanchezv = 0;
     bank_latch_jsanchezv = 0; gb_ptr_IdRomRam_jsanchezv[3] = 0;
     paging_lock_jsanchezv = 1;
     #ifdef use_optimice_writebyte
      gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[0];
      gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
      gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
     #endif     
    }
    //size_read = 0; //No se usa

    //int leidos=0;    
   unsigned short int leidos=0;
   unsigned short int bytesLeer= 1024; //Leemos 1024 bytes
   unsigned short int timeout_uart_sna= 2000; //2000 ms
   unsigned char isTimeOut=0;    
   leidos= Leer_stream_UART(bytesLeer,timeout_uart_sna,&isTimeOut);    
   if (leidos>0)
   {

    // Read in the registers        
    cpu.setRegI(gb_buffer_uart_dest[0]);
    cpu.setRegLx(gb_buffer_uart_dest[1]);
    cpu.setRegHx(gb_buffer_uart_dest[2]);
    cpu.setRegEx(gb_buffer_uart_dest[3]);
    cpu.setRegDx(gb_buffer_uart_dest[4]);
    cpu.setRegCx(gb_buffer_uart_dest[5]);
    cpu.setRegBx(gb_buffer_uart_dest[6]);
    cpu.setRegFx(gb_buffer_uart_dest[7]);
    cpu.setRegAx(gb_buffer_uart_dest[8]);

    cpu.setRegL(gb_buffer_uart_dest[9]);
    cpu.setRegH(gb_buffer_uart_dest[10]);
    cpu.setRegE(gb_buffer_uart_dest[11]);
    cpu.setRegD(gb_buffer_uart_dest[12]);
    cpu.setRegC(gb_buffer_uart_dest[13]);
    cpu.setRegB(gb_buffer_uart_dest[14]);    

    sp_l= gb_buffer_uart_dest[15];
    sp_h= gb_buffer_uart_dest[16];    
    cpu.setRegIY(sp_l + sp_h * 0x100);
    sp_l= gb_buffer_uart_dest[17];
    sp_h= gb_buffer_uart_dest[18];
    cpu.setRegIX(sp_l + sp_h * 0x100);
    byte inter = gb_buffer_uart_dest[19];//lhandle.read();
    auxIFF2 = (inter & 0x04) ? true : false;
    cpu.setIFF2(auxIFF2);
    cpu.setRegR(gb_buffer_uart_dest[20]);
    cpu.setFlags(gb_buffer_uart_dest[21]);
    cpu.setRegA(gb_buffer_uart_dest[22]);
    sp_l = gb_buffer_uart_dest[23];//lhandle.read();
    sp_h = gb_buffer_uart_dest[24];//lhandle.read();
    cpu.setRegSP(sp_l + sp_h * 0x100);

    switch (gb_buffer_uart_dest[25])
    {
     case 0: cpu.setIM(cpu.IM0); break;
     case 1: cpu.setIM(cpu.IM1); break;
     case 2: cpu.setIM(cpu.IM2); break;
	  }

    byte bordercol = gb_buffer_uart_dest[26];//lhandle.read();
    borderTemp_jsanchezv = bordercol;
    cpu.setIFF1(auxIFF2);

    uint16_t thestack = cpu.getRegSP();
    uint16_t buf_p = 0x4000;
    contBuffer = 27;
    
    //He leido 1024 bytes. Lee resto 27
    int cont1024= 27;
    //while (contBuffer< 50000)
    while (contBuffer< SIZE_SNA_48K)
    {
     //JJ writebyte(buf_p, lhandle.read());
     poke8(buf_p, gb_buffer_uart_dest[cont1024]);
     contBuffer++;
     buf_p++;
     
     cont1024++;
     if (cont1024 >= 1024)
     {
      isTimeOut=0;
      bytesLeer= (SIZE_SNA_48K-contBuffer)>=1024 ? 1024 : (SIZE_SNA_48K-contBuffer);
      leidos= Leer_stream_UART(bytesLeer,timeout_uart_sna,&isTimeOut);
      if (leidos>0)
      {
       sprintf(cadout,"%05d/%05d",SIZE_SNA_48K,contBuffer);
       cadout[12]='\0';
       SDLprintText(cadout,50,20,7,0);
      }
      cont1024= 0;
     }
     if (isTimeOut)
     {
      break;
     }     
    }    
    retaddr = peek16(thestack);    
    unsigned short int auxSP = cpu.getRegSP();
    auxSP++;
    auxSP++;
    cpu.setRegSP(auxSP);
    cpu.setRegPC(retaddr);
   }
  }
 #endif

 #ifndef use_lib_wifi
 //Lee SNA desde flash core jsanchez
 void Z80sim::load_ram2Flash_jsanchezv(unsigned char id,unsigned char isSNA48K)
 {
  if (isSNA48K != 1)
  {
   load_ram2Flash128_jsanchezv(id);
   return;
  }

    int contBuffer=0; 
    //uint16_t size_read; //No se usa
    byte sp_h, sp_l;
    uint16_t retaddr; 
	bool auxIFF2=false;
	//unsigned char auxSwap;  //No se usa
    
    memset(ram0_jsanchezv,0,0x4000);
    memset(ram1_jsanchezv,0,0x4000);
    memset(ram2_jsanchezv,0,0x4000);
    memset(ram3_jsanchezv,0,0x4000);
    memset(ram4_jsanchezv,0,0x4000);
    memset(ram5_jsanchezv,0,0x4000);
    memset(ram6_jsanchezv,0,0x4000);
    memset(ram7_jsanchezv,0,0x4000);

    //ResetSound();
    
    if (id >= max_list_sna_48)
     return;
            
    cpu.reset(); //zx_reset();        
    //Hago lo mismo que zx_reset
    zx_reset_jsanchezv();    
    //Fin lo mismo zx_reset

    if (gb_cfg_arch_is48K == 1)
    {
     rom_latch_jsanchezv = 0;
     rom_in_use_jsanchezv = 0;
     bank_latch_jsanchezv = 0; gb_ptr_IdRomRam_jsanchezv[3] = 0;
     paging_lock_jsanchezv = 1;
     #ifdef use_optimice_writebyte
      gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[0];
      gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
      gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
     #endif     
    }
    //size_read = 0; //No se usa
    // Read in the registers        
    cpu.setRegI(gb_list_sna_48k_data[id][0]);
    cpu.setRegLx(gb_list_sna_48k_data[id][1]);
    cpu.setRegHx(gb_list_sna_48k_data[id][2]);
    cpu.setRegEx(gb_list_sna_48k_data[id][3]);
    cpu.setRegDx(gb_list_sna_48k_data[id][4]);
    cpu.setRegCx(gb_list_sna_48k_data[id][5]);
    cpu.setRegBx(gb_list_sna_48k_data[id][6]);
    cpu.setRegFx(gb_list_sna_48k_data[id][7]);
    cpu.setRegAx(gb_list_sna_48k_data[id][8]);

    cpu.setRegL(gb_list_sna_48k_data[id][9]);
    cpu.setRegH(gb_list_sna_48k_data[id][10]);
    cpu.setRegE(gb_list_sna_48k_data[id][11]);
    cpu.setRegD(gb_list_sna_48k_data[id][12]);
    cpu.setRegC(gb_list_sna_48k_data[id][13]);
    cpu.setRegB(gb_list_sna_48k_data[id][14]);    

    sp_l= gb_list_sna_48k_data[id][15];
    sp_h= gb_list_sna_48k_data[id][16];    
    cpu.setRegIY(sp_l + sp_h * 0x100);
    sp_l= gb_list_sna_48k_data[id][17];
    sp_h= gb_list_sna_48k_data[id][18];
    cpu.setRegIX(sp_l + sp_h * 0x100);
    byte inter = gb_list_sna_48k_data[id][19];//lhandle.read();
    auxIFF2 = (inter & 0x04) ? true : false;
    cpu.setIFF2(auxIFF2);
    cpu.setRegR(gb_list_sna_48k_data[id][20]);
    cpu.setFlags(gb_list_sna_48k_data[id][21]);
    cpu.setRegA(gb_list_sna_48k_data[id][22]);
    sp_l = gb_list_sna_48k_data[id][23];//lhandle.read();
    sp_h = gb_list_sna_48k_data[id][24];//lhandle.read();
    cpu.setRegSP(sp_l + sp_h * 0x100);

    switch (gb_list_sna_48k_data[id][25])
    {
     case 0: cpu.setIM(cpu.IM0); break;
     case 1: cpu.setIM(cpu.IM1); break;
     case 2: cpu.setIM(cpu.IM2); break;
	}

    byte bordercol = gb_list_sna_48k_data[id][26];//lhandle.read();
    borderTemp_jsanchezv = bordercol;
    cpu.setIFF1(auxIFF2);

    uint16_t thestack = cpu.getRegSP();
    uint16_t buf_p = 0x4000;
    contBuffer = 27;
    //while (contBuffer< 50000)        
    while (contBuffer< SIZE_SNA_48K)
    {
     //JJ writebyte(buf_p, lhandle.read());
     poke8(buf_p, gb_list_sna_48k_data[id][contBuffer]);
     contBuffer++;
     buf_p++;
    }    
    retaddr = peek16(thestack);    
    unsigned short int auxSP = cpu.getRegSP();
    auxSP++;
    auxSP++;
    cpu.setRegSP(auxSP);
    cpu.setRegPC(retaddr);    
 }
 #endif


 #ifdef use_lib_wifi
 //Lee SNA desde WIFI core jsanchez
 void Z80sim::load_ram2Flash_jsanchezvFromWIFI(char * cadUrl,unsigned char isSNA48K)
 {
  #ifdef use_lib_wifi_debug
   Serial.printf("load_ram2Flash_jsanchezvFromWIFI\r\n");
  #endif     
  if (isSNA48K != 1)
  {
   //load_ram2Flash128_jsanchezv(id);
   return;
  }

    int contBuffer=0; 
    uint16_t size_read;
    byte sp_h, sp_l;
    uint16_t retaddr; 
	bool auxIFF2=false;
	unsigned char auxSwap; 
    
    #ifdef use_lib_only_48k
     memset(ram0_jsanchezv,0,0x4000);
     memset(ram2_jsanchezv,0,0x4000);
     memset(ram5_jsanchezv,0,0x4000);
    #else
     memset(ram0_jsanchezv,0,0x4000);
     memset(ram1_jsanchezv,0,0x4000);
     memset(ram2_jsanchezv,0,0x4000);
     memset(ram3_jsanchezv,0,0x4000);
     memset(ram4_jsanchezv,0,0x4000);
     memset(ram5_jsanchezv,0,0x4000);
     memset(ram6_jsanchezv,0,0x4000);
     memset(ram7_jsanchezv,0,0x4000);
    #endif 

    //ResetSound();
    
    //if (id >= max_list_sna_48)
    // return;
            
    cpu.reset(); //zx_reset();        
    //Hago lo mismo que zx_reset
    zx_reset_jsanchezv();    
    //Fin lo mismo zx_reset

    if (gb_cfg_arch_is48K == 1)
    {
     rom_latch_jsanchezv = 0;
     rom_in_use_jsanchezv = 0;
     bank_latch_jsanchezv = 0; gb_ptr_IdRomRam_jsanchezv[3] = 0;
     paging_lock_jsanchezv = 1;
     #ifdef use_optimice_writebyte
      gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[0];
      gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
      gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
     #endif     
    }
    size_read = 0;

    #ifdef use_lib_wifi_debug
     Serial.printf("Check WIFI\r\n");
    #endif 
    if (Check_WIFI() == false)
    {
     return;
    }
    int leidos=0;
    #ifdef use_lib_wifi_debug
     Serial.printf("URL:%s\r\n",cadUrl);
    #endif 
    Asignar_URL_stream_WIFI(cadUrl);    
    Leer_url_stream_WIFI(&leidos);
    #ifdef use_lib_wifi_debug
     Serial.printf("Leidos:%d\r\n",leidos); //Leemos 1024 bytes
    #endif    

    // Read in the registers        
    cpu.setRegI(gb_buffer_wifi[0]);
    cpu.setRegLx(gb_buffer_wifi[1]);
    cpu.setRegHx(gb_buffer_wifi[2]);
    cpu.setRegEx(gb_buffer_wifi[3]);
    cpu.setRegDx(gb_buffer_wifi[4]);
    cpu.setRegCx(gb_buffer_wifi[5]);
    cpu.setRegBx(gb_buffer_wifi[6]);
    cpu.setRegFx(gb_buffer_wifi[7]);
    cpu.setRegAx(gb_buffer_wifi[8]);

    cpu.setRegL(gb_buffer_wifi[9]);
    cpu.setRegH(gb_buffer_wifi[10]);
    cpu.setRegE(gb_buffer_wifi[11]);
    cpu.setRegD(gb_buffer_wifi[12]);
    cpu.setRegC(gb_buffer_wifi[13]);
    cpu.setRegB(gb_buffer_wifi[14]);    

    sp_l= gb_buffer_wifi[15];
    sp_h= gb_buffer_wifi[16];    
    cpu.setRegIY(sp_l + sp_h * 0x100);
    sp_l= gb_buffer_wifi[17];
    sp_h= gb_buffer_wifi[18];
    cpu.setRegIX(sp_l + sp_h * 0x100);
    byte inter = gb_buffer_wifi[19];//lhandle.read();
    auxIFF2 = (inter & 0x04) ? true : false;
    cpu.setIFF2(auxIFF2);
    cpu.setRegR(gb_buffer_wifi[20]);
    cpu.setFlags(gb_buffer_wifi[21]);
    cpu.setRegA(gb_buffer_wifi[22]);
    sp_l = gb_buffer_wifi[23];//lhandle.read();
    sp_h = gb_buffer_wifi[24];//lhandle.read();
    cpu.setRegSP(sp_l + sp_h * 0x100);

    switch (gb_buffer_wifi[25])
    {
     case 0: cpu.setIM(cpu.IM0); break;
     case 1: cpu.setIM(cpu.IM1); break;
     case 2: cpu.setIM(cpu.IM2); break;
	}

    byte bordercol = gb_buffer_wifi[26];//lhandle.read();
    borderTemp_jsanchezv = bordercol;
    cpu.setIFF1(auxIFF2);

    uint16_t thestack = cpu.getRegSP();
    uint16_t buf_p = 0x4000;
    contBuffer = 27;
    
    //He leido 1024 bytes. Lee resto 27
    int cont1024= 27;
    //while (contBuffer< 50000)
    while (contBuffer< SIZE_SNA_48K)
    {
     //JJ writebyte(buf_p, lhandle.read());
     poke8(buf_p, gb_buffer_wifi[cont1024]);
     contBuffer++;
     buf_p++;
     
     cont1024++;
     if (cont1024 >= 1024)
     {
      Leer_url_stream_WIFI(&leidos);
      #ifdef use_lib_wifi_debug
       Serial.printf("Leidos:%d\r\n",leidos);
      #endif 
      cont1024= 0;
     }     
    }    
    retaddr = peek16(thestack);    
    unsigned short int auxSP = cpu.getRegSP();
    auxSP++;
    auxSP++;
    cpu.setRegSP(auxSP);
    cpu.setRegPC(retaddr);    
 } 
 #endif 

 Z80sim::Z80sim(void) : cpu(this)
 {

 }

 Z80sim::~Z80sim() {}

 //void Z80sim::AssignPtrRamPort(unsigned char *ptrRam,unsigned char *ptrPort)
 //{
 // z80Ram = ptrRam;
 // z80Ports = ptrPort;
 //}

uint8_t Z80sim::fetchOpcode(uint16_t address) {
 // 3 clocks to fetch opcode from RAM and 1 execution clock
 tstates += 4;
 return peek8(address);
 //unsigned char idRomRam = (address>>14);
 //unsigned char aux_fetchOpcode=0; 
 //if (idRomRam == 0)
 // aux_fetchOpcode= rom0_jsanchezv[address];
 //else
 // aux_fetchOpcode = z80Ram[address]; 
 // 
 //printf ("fetchOpcode:%02x add:%d",aux_fetchOpcode,address);
 //return aux_fetchOpcode;

//JJ  #ifdef WITH_BREAKPOINT_SUPPORT
//JJ   return z80Ram[address];
//JJ  #else
//JJ   uint8_t opcode = z80Ram[address];
//JJ   return (address != 0x0005 ? opcode : breakpoint(address, opcode));
//JJ  #endif
}

#ifdef use_optimice_writebyte
 unsigned char * gb_real_ptr_ram[4];
 const unsigned char * gb_real_ptr_rom[4]; 
 unsigned char * gb_real_read_ptr_ram[4];
 unsigned char * gb_real_write_ptr_ram[4];
 #ifdef use_optimice_writebyte_min_sram
  unsigned char ramFast[2];
 #else  
  unsigned char ramFast[0x4000];
 #endif 

 void Z80sim::AsignarRealPtrRAM()
 {//ram0 ram5 ram2 ram0
  //gb_real_ptr_ram[0]= ram0;
  memset(ramFast,0,sizeof(ramFast));
  gb_real_ptr_ram[0]= ramFast;
  gb_real_ptr_ram[1]= ram5_jsanchezv;
  gb_real_ptr_ram[2]= ram2_jsanchezv;
  gb_real_ptr_ram[3]= ram0_jsanchezv;
  
  
  gb_real_write_ptr_ram[0]= ramFast; //write
  gb_real_write_ptr_ram[1]= ram5_jsanchezv;
  gb_real_write_ptr_ram[2]= ram2_jsanchezv;
  gb_real_write_ptr_ram[3]= ram0_jsanchezv;
  
  gb_real_read_ptr_ram[0]= (unsigned char *)rom0_jsanchezv; //read
  gb_real_read_ptr_ram[1]= ram5_jsanchezv;
  gb_real_read_ptr_ram[2]= ram2_jsanchezv;
  gb_real_read_ptr_ram[3]= ram0_jsanchezv;
 }
#endif 

#ifdef use_optimice_writebyte
 inline uint8_t Z80sim::peek8(uint16_t address) 
 {
  tstates += 3;
  return (gb_real_read_ptr_ram[(address>>14)][(address & 0x3fff)]);
 }
#else
 uint8_t Z80sim::peek8(uint16_t address) 
 {
  // 3 clocks for read byte from RAM
  tstates += 3;
  unsigned char idRomRam = (address>>14);
  if (idRomRam == 0)
   return (gb_local_cache_rom_jsanchezv[rom_in_use_jsanchezv][address]);
  else
   return (gb_local_cache_ram_jsanchezv[(gb_ptr_IdRomRam_jsanchezv[idRomRam])][(address & 0x3fff)]);     
  //JJ return z80Ram[address];    
  //printf ("peek8 add:%d\n",address);
  //unsigned char idRomRam = (address>>14);
  //if (idRomRam == 0)
  // return (rom0_jsanchezv[address]);
  //else
  // return (z80Ram[address]);
   
 // switch(idRomRam)
 // {
 //  case 0: return (rom0_jsanchezv[address]);  break;
 //  case 1: 
 //   //printf ("rram5 add:%d\n",address);
 //   return ram5_jsanchezv[(address & 0x3fff)]; 
 //   break;
 //  case 2:
 //   //printf ("rram2 add:%d\n",address); 
 //   return ram2_jsanchezv[(address & 0x3fff)]; 
 //   break;
 //  case 3: 
 //   //printf ("rram:%d\n",address); 
 //   return (z80Ram[address]);
 //   break;
 // }
 }
#endif

//Captura direccion memoria estados
#ifdef use_lib_cycle_32bits_jsanchezv
 unsigned int * Z80sim::GetAddr_tstates(void)
 {
  return &tstates;
 }
#else
 uint64_t * Z80sim::GetAddr_tstates(void)
 {
  return &tstates;
 }
#endif 

#ifdef use_optimice_writebyte
 inline void Z80sim::poke8(uint16_t address, uint8_t value)
 {
  #ifdef use_optimice_writebyte_min_sram
   unsigned char idRomRam = (address>>14);
   tstates += 3;
   gb_real_write_ptr_ram[idRomRam][(idRomRam==0) ? 0 : (address & 0x3fff)] = value;
  #else
   tstates += 3;  
   gb_real_write_ptr_ram[(address>>14)][(address & 0x3fff)] = value;
  #endif 
 }
#else
 void Z80sim::poke8(uint16_t address, uint8_t value)
 {
  // 3 clocks for write byte to RAM
  tstates += 3;
  unsigned char idRomRam = (address>>14);
  if (idRomRam != 0)
  {
   gb_local_cache_ram_jsanchezv[(gb_ptr_IdRomRam_jsanchezv[idRomRam])][(address & 0x3fff)] = value;   
  }
  //JJ z80Ram[address] = value;
  //printf ("poke8 add:%d\n",address);
  //unsigned char idRomRam = (address>>14);
  //if (idRomRam != 0)
  // z80Ram[address] = value;
  
  //switch(idRomRam)
  //{
  // //case 0: return (rom0_jsanchezv[address]);  break;
  // case 1: 
  //  //if (value!=0)
  //  // printf ("wram5 add:%d val:%d add:%d\n",address,value,(address & 0x3fff)); 
  //  ram5_jsanchezv[(address & 0x3fff)] = value;
  //  break;
  // case 2: 
  //  //printf ("wram2 add:%d\n",address);
  //  ram2_jsanchezv[(address & 0x3fff)] = value; 
  //  break;
  // case 3:
  //  //printf ("wram:%d\n",address);
  //  z80Ram[address] = value; 
  //  break;
  //}  
 }
#endif 

uint16_t Z80sim::peek16(uint16_t address) {
 //printf ("peek16 add:%d\n",address);
    // Order matters, first read lsb, then read msb, don't "optimize"
    uint8_t lsb = peek8(address);
    uint8_t msb = peek8(address + 1);
    return (msb << 8) | lsb;
}

void Z80sim::poke16(uint16_t address, RegisterPair word) {
 //printf ("poke16 add:%d\n",address);     
    // Order matters, first write lsb, then write msb, don't "optimize"
    poke8(address, word.byte8.lo);
    poke8(address + 1, word.byte8.hi);
}

uint8_t Z80sim::inPort(uint16_t port) {
    // 4 clocks for read byte from bus
 //printf ("inPort port:%d\n",port);    
    tstates += 3;
 //int16_t kbdarrno = 0; //No se usa
 unsigned portLow= port & 0xFF;
 unsigned portHigh= (port>>8) & 0xFF;

 
 if (
     (portLow == 0xDF)
     && 
     ((portHigh == 0xFA)||(portHigh == 0xFB)||(portHigh == 0xFF))
    )
    {
     //printf("Kempston H:%x L:%x %d %d %d\n",portHigh,portLow,gb_z80_mouse_x,gb_z80_mouse_y,gb_z80_mouseBtn);
     switch (portHigh)
     {
      case 0xFB: break;
      case 0xFF: break;
      case 0xFA: break;
      case 0xFE: break; //Detect mouse OK
      default: return 0xFF;
     }
    }
    
 
 if (portLow == 0xFE)
 {
  uint8_t result = 0xFF;
  if (~(portHigh | 0xFE)&0xFF) result &= (z80Ports_jsanchezv[0]); //JJ teclado
  if (~(portHigh | 0xFD)&0xFF) result &= (z80Ports_jsanchezv[1]);
  if (~(portHigh | 0xFB)&0xFF) result &= (z80Ports_jsanchezv[2]);
  if (~(portHigh | 0xF7)&0xFF) result &= (z80Ports_jsanchezv[3]);
  if (~(portHigh | 0xEF)&0xFF) result &= (z80Ports_jsanchezv[4]);
  if (~(portHigh | 0xDF)&0xFF) result &= (z80Ports_jsanchezv[5]);
  if (~(portHigh | 0xBF)&0xFF) result &= (z80Ports_jsanchezv[6]);
  if (~(portHigh | 0x7F)&0xFF) result &= (z80Ports_jsanchezv[7]);
  return result;
 }
 // Kempston
 if (portLow == 0x1F) {
  return z80Ports_jsanchezv[31];
 } 
 
 uint8_t data = zx_data_jsanchezv;
 data |= (0xe0); /* Set bits 5-7 - as reset above */
 data &= ~0x40;
 // Serial.printf("Port %x%x  Data %x\n", portHigh,portLow,data);;
 return data;
}

void Z80sim::outPort(uint16_t port, uint8_t value) {
 //printf ("outPort outPort:%d\n",port);         
    // 4 clocks for write byte to bus
    tstates += 4;
 
 unsigned char tmp_data = value;
 unsigned portLow= port & 0xFF;
 unsigned portHigh= ((port>>8) & 0xFF);
    
 tstates += 4;
 //JJ z80Ports[port] = value;
 
 switch (portLow)
 {
  case 0xFE:
   bitWrite(borderTemp_jsanchezv, 0, bitRead(value, 0));
   bitWrite(borderTemp_jsanchezv, 1, bitRead(value, 1));
   bitWrite(borderTemp_jsanchezv, 2, bitRead(value, 2));
   gbSoundSpeaker= (bitRead(value, 4) | bitRead(value, 3));//mic and spk
   z80Ports_jsanchezv[0x20] = value;
   break;
   
  case 0xFD:
   switch (portHigh)
   {
    case 0x7F:
     //cambio banco
     if (!paging_lock_jsanchezv)
     {
      paging_lock_jsanchezv = bitRead(tmp_data, 5);
      rom_latch_jsanchezv = bitRead(tmp_data, 4);      
      video_latch_jsanchezv = bitRead(tmp_data, 3);
      bank_latch_jsanchezv = tmp_data & 0x7; 
      gb_ptr_IdRomRam_jsanchezv[3]= bank_latch_jsanchezv;
      // rom_in_use=0;
      bitWrite(rom_in_use_jsanchezv, 1, sp3_rom_jsanchezv);
      bitWrite(rom_in_use_jsanchezv, 0, rom_latch_jsanchezv);
      #ifdef use_optimice_writebyte
       gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[rom_in_use_jsanchezv];
       gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[bank_latch_jsanchezv];
       gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[bank_latch_jsanchezv];
      #endif      
     }
    
    
     break;
    
    case 0x1F:
     sp3_mode_jsanchezv = bitRead(value, 0);
     sp3_rom_jsanchezv = bitRead(value, 2);
     bitWrite(rom_in_use_jsanchezv, 1, sp3_rom_jsanchezv);
     bitWrite(rom_in_use_jsanchezv, 0, rom_latch_jsanchezv);
     #ifdef use_optimice_writebyte
      gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[rom_in_use_jsanchezv];
     #endif     
     // Serial.printf("1FFD data: %x mode: %x rom bits: %x ROM chip: %x\n",data,sp3_mode,sp3_rom, rom_in_use);
     break;        
   }       
   break;
 }//fin switch
}

void Z80sim::addressOnBus(uint16_t address, int32_t tstates) {
 //printf ("addressOnBus:%d\n",address);
    // Additional clocks to be added on some instructions
    this->tstates += tstates;
}

void Z80sim::interruptHandlingTime(int32_t tstates) {
// printf ("interruptHandlingTime\n");
    this->tstates += tstates;
}

bool Z80sim::isActiveINT(void) {
// printf ("isActiveINT\n");     
	// Put here the needed logic to trigger an INT
 if (interruptPend_jsanchezv == 0) return false;
 interruptPend_jsanchezv = 0;
 return true;
}

#ifdef WITH_EXEC_DONE
void Z80sim::execDone(void) {}
#endif

uint8_t Z80sim::breakpoint(uint16_t address, uint8_t opcode) {
    // Emulate CP/M Syscall at address 5

#ifdef WITH_BREAKPOINT_SUPPORT
    if (address != 0x0005)
         return opcode;
#endif

    //printf ("cpu.getRegC %d opcode:%d\n",cpu.getRegC(),opcode);
    switch (cpu.getRegC()) {
        case 0: // BDOS 0 System Reset
        {
            //JJcout << "Z80 reset after " << tstates << " t-states" << endl;
            //printf ("Z80 reset after %d t-states\n",tstates);
            finish = true;
            break;
        }
        case 2: // BDOS 2 console char output
        {
            //JJ cout << (char) cpu.getRegE();
            //printf("BDOS 2 %c",(char) cpu.getRegE());
            break;
        }
        case 9: // BDOS 9 console string output (string terminated by "$")
        {
         char jj_cad[128]="";
         int jj_cont_cad=0;
         memset(jj_cad,0,50);
         jj_cad[0]='\0';
         
            uint16_t strAddr = cpu.getRegDE();
            while (peek8(strAddr) != '$')
            {
             //cout << (char) z80Ram[strAddr++];
             jj_cad[jj_cont_cad]=(char)peek8(strAddr);
             strAddr++;
             if (jj_cont_cad>=50)
              break;
             if (jj_cont_cad<50)
              jj_cont_cad++;             
            }
            jj_cad[jj_cont_cad]='\0';
            //printf("BDOS 9 len:%d dat:%s\n",jj_cont_cad,jj_cad);
            //cout.flush();
            break;
        }
        default:
        {
            //cout << "BDOS Call " << cpu.getRegC() << endl;
            //printf("BDOS Call %c\n",cpu.getRegC());
            finish = true;
            //printf("finish\n");
            //cout << finish << endl;
        }
    }
    // opcode would be modified before the decodeOpcode method
    return opcode;
}

void Z80sim::ResetCPU()
{
 cpu.reset();
  
 bank_latch_jsanchezv=0;
 video_latch_jsanchezv = 0;
 rom_latch_jsanchezv = 0;
 paging_lock_jsanchezv = 0;
 rom_in_use_jsanchezv= 0;
 sp3_rom_jsanchezv = 0;
 sp3_mode_jsanchezv = 0;
 borderTemp_jsanchezv=7;
 #ifdef use_optimice_writebyte
  gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[0];
  gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
  gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
 #endif   
 //printf ("Reset desde ResetCPU\n");
 //fflush(stdout); 
}


void Z80sim::runTestJJ_poll(void)
{
  cpu.jj_execute_one_frame(); //69887 states one frame
  //gb_time_state_jsanchez_ini= this->tstates;
  //vsync_jsanchezv= 1;
  //gb_fps_jsanchezv++;
  //interruptPend_jsanchezv=1;


   //gb_time_state_jsanchez_ini= this->tstates;
   //gb_time_state_jsanchez_ini= *gb_addr_states_jsanchezv;
   //vsync_jsanchezv= 1;
   interruptPend_jsanchezv=1;
   #ifdef use_lib_stats_time_unified    
    //gb_fps_jsanchezv++;
    gb_fps_unified++;
    ahora_jsanchezv= micros();
    //Serial.printf("CPU %d\n",(ahora_jsanchezv-antes_jsanchezv));
    gb_stats_time_cur_unified= (ahora_jsanchezv-antes_jsanchezv);
    if (gb_stats_time_cur_unified < gb_stats_time_min_unified)
     gb_stats_time_min_unified= gb_stats_time_cur_unified;
    if (gb_stats_time_cur_unified > gb_stats_time_max_unified)
     gb_stats_time_max_unified= gb_stats_time_cur_unified;
    antes_jsanchezv= ahora_jsanchezv;
   #endif

/*
  while (((this->tstates - gb_time_state_jsanchez_ini)<=69887))
  {
   cpu.execute();
  }
  //if ((this->tstates - gb_time_state_jsanchez_ini)>69887)
  //{   
   gb_time_state_jsanchez_ini= this->tstates;
   vsync_jsanchezv= 1;
   interruptPend_jsanchezv=1;
   #ifdef use_lib_stats_time_jsanchezv    
    gb_fps_jsanchezv++;
    ahora_jsanchezv= micros();
    //Serial.printf("CPU %d\n",(ahora_jsanchezv-antes_jsanchezv));
    gb_stats_time_cur_jsanchezv= (ahora_jsanchezv-antes_jsanchezv);
    if (gb_stats_time_cur_jsanchezv < gb_stats_time_min_jsanchezv)
     gb_stats_time_min_jsanchezv=gb_stats_time_cur_jsanchezv;
    if (gb_stats_time_cur_jsanchezv > gb_stats_time_max_jsanchezv)
     gb_stats_time_max_jsanchezv = gb_stats_time_cur_jsanchezv;
    antes_jsanchezv= ahora_jsanchezv;
   #endif
  //}
*/
 //if (!finish) {
 // cpu.execute();
  //printf("1\n");
  //printf("gb_cont: %d\n",gb_cont);
  //gb_cont++;
// }
}

void Z80sim::runTestJJ(void)
{
  //zx modo 48K rom 0, ram0, ram2, y ram5
  #ifdef use_lib_rom0_inRAM_jsanchezv    
   rom0_inRAM_jsanchezv = (unsigned char *)malloc(0x4000);
  #endif
  #ifdef use_lib_only_48k
   ram1_jsanchezv = NULL;
   ram3_jsanchezv = NULL;
   ram4_jsanchezv = NULL;
  #else
   ram1_jsanchezv = (unsigned char *)malloc(0x4000);  
   ram3_jsanchezv = (unsigned char *)malloc(0x4000);
   ram4_jsanchezv = (unsigned char *)malloc(0x4000);   
  #endif
  #ifdef use_lib_48k_iram_jsanchezv   
  #else
   ram0_jsanchezv = (unsigned char *)malloc(0x4000);
   ram2_jsanchezv = (unsigned char *)malloc(0x4000);
   ram5_jsanchezv = (unsigned char *)malloc(0x4000);
  #endif
 
  #ifdef use_lib_only_48k
   ram6_jsanchezv = NULL;
   ram7_jsanchezv = NULL;
  #else
   ram6_jsanchezv = (unsigned char *)malloc(0x4000);
   ram7_jsanchezv = (unsigned char *)malloc(0x4000);
  #endif 
  
  z80Ports_jsanchezv = (unsigned char *)malloc(256);
  memset(z80Ports_jsanchezv, 0x1F, 256);   

  #ifdef use_lib_only_48k
   memset(ram0_jsanchezv,0,0x4000);
   memset(ram2_jsanchezv,0,0x4000);
   memset(ram5_jsanchezv,0,0x4000);
  #else
   memset(ram0_jsanchezv,0,0x4000);
   memset(ram1_jsanchezv,0,0x4000);
   memset(ram2_jsanchezv,0,0x4000);
   memset(ram3_jsanchezv,0,0x4000);
   memset(ram4_jsanchezv,0,0x4000);
   memset(ram5_jsanchezv,0,0x4000);
   memset(ram6_jsanchezv,0,0x4000);
   memset(ram7_jsanchezv,0,0x4000);
  #endif

  #ifdef use_lib_48k_iram_jsanchezv
   AssignStaticRam_jsanchezv(ram0_jsanchezv,ram2_jsanchezv,ram5_jsanchezv);//fast iram
  #else
  #endif 
  AsignarRom_jsanchezv(); 
  ReloadLocalCacheROMram_jsanchezv();
  bank_latch_jsanchezv=0;
  video_latch_jsanchezv = 0;
  rom_latch_jsanchezv = 0;
  #ifdef use_optimice_writebyte
   gb_real_read_ptr_ram[0]= (unsigned char*)gb_local_cache_rom_jsanchezv[0];
   gb_real_read_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
   gb_real_write_ptr_ram[3]= gb_local_cache_ram_jsanchezv[0];
  #endif  
  paging_lock_jsanchezv = 0;
  rom_in_use_jsanchezv= 0;
  sp3_rom_jsanchezv = 0;
  sp3_mode_jsanchezv = 0;
  borderTemp_jsanchezv=7;  

  #ifdef use_optimice_writebyte
   #ifdef use_lib_log_serial
    Serial.printf("Optimice writebyte Setup AsignarRealPtrRAM\n");
   #endif 
   AsignarRealPtrRAM();   
  #endif       

  cpu.reset();
  gb_time_state_jsanchez_ini = this->tstates;

 //memset(z80Ram,0,sizeof z80Ram);
 //memset(z80Ports,0,sizeof z80Ports);
// memcpy(&z80Ram[0x100],gb_programa,8590);
 
//    cpu.reset();
//    finish = false;
    
  //cpu.setRegPC(0x3B5);
//  memcpy(z80Ram,rom0_jsanchezv,16384);
 // for (int i=0; i<50;i++)
 //  printf ("%02x",rom0_jsanchezv[i]);
 // printf("\n"); 

/*    z80Ram[0] = (uint8_t) 0xC3;
    z80Ram[1] = 0x00;
    z80Ram[2] = 0x01; // JP 0x100 CP/M TPA
    z80Ram[5] = (uint8_t) 0xC9; // Return from BDOS call

    //cpu.setBreakpoint(0x0005, true);
    #ifdef WITH_BREAKPOINT_SUPPORT
     cpu.setBreakpoint(true);
     printf("cpu.setBreakpoint(true)\n");
    #endif    */
    //while (!finish) {
    //    cpu.execute();
    //} 
}

/*void Z80sim::runTest(std::ifstream* f) {
    streampos size;
    if (!f->is_open()) {
        cout << "file NOT OPEN" << endl;
        return;
    } else cout << "file open" << endl;

    size = f->tellg();
    cout << "Test size: " << size << endl;
    f->seekg(0, ios::beg);
    f->read((char *) &z80Ram[0x100], size);
    f->close();

#ifdef WITH_BREAKPOINT_SUPPORT
    cpu.setBreakpoint(true);
#endif

    cpu.reset();
    finish = false;

    z80Ram[0] = (uint8_t) 0xC3;
    z80Ram[1] = 0x00;
    z80Ram[2] = 0x01; // JP 0x100 CP/M TPA
    z80Ram[5] = (uint8_t) 0xC9; // Return from BDOS call

    while (!finish) {
        cpu.execute();
    }
}*/

 Z80sim sim = Z80sim();
 Z80sim * gb_ptrSim= &sim;
 #ifdef use_lib_cycle_32bits_jsanchezv
  unsigned int * gb_addr_states_jsanchezv = sim.GetAddr_tstates();
 #else
  uint64_t * gb_addr_states_jsanchezv = sim.GetAddr_tstates();
 #endif 
#endif


#ifdef use_lib_core_jsanchezv
 //Emulacion de jsanchezv()
 void loop_jsanchezv()
 {
  //printf("loop_jsanchezv\n");
  sim.runTestJJ_poll();
 }
#endif

//********************************************************************
void PrepareColorsUltraFastVGA()
{  
 //(color & RGBAXMask) | SBits;
 #ifdef use_lib_vga8colors
  for (unsigned char i=0;i<8;i++){   
   gb_cache_zxcolor[i]= (gb_cache_zxcolor[i] & 0x07) | gb_sync_bits;
   #ifdef use_lib_use_bright
    gb_cache_zxcolor_bright[i]= gb_cache_zxcolor[i];
   #endif 
  }
 #else
  for (unsigned char i=0;i<8;i++){//DAC 6 bits 64 colores   
   gb_cache_zxcolor[i]= (gb_cache_zxcolor[i] & 0x3F) | gb_sync_bits;   
   gb_cache_zxcolor_bright[i]= (gb_cache_zxcolor_bright[i] & 0x3F) | gb_sync_bits;   
  }
 #endif
}




void setup()
{
 //DO NOT turn off peripherals to recover some memory
 //esp_bt_controller_deinit(); //Reduzco consumo RAM
 //esp_bt_controller_mem_release(ESP_BT_MODE_BTDM); //Reduzco consumo RAM 
 #if defined(use_lib_log_serial) || defined(use_lib_keyboard_uart)
  Serial.begin(115200);
 #endif

 #ifdef use_lib_log_serial
  Serial.printf("HEAP BEGIN %d\r\n", ESP.getFreeHeap());
  //JJSerial.printf("PSRAM size: %d\n", ESP.getPsramSize());
 #endif

 #ifdef use_lib_keyboard_uart
  Serial.setTimeout(use_lib_keyboard_uart_timeout);
 #endif

 #ifdef use_lib_lookup_ram
  gb_lookup_calcY[0]= 0; //force ram compiler,not const progmem
 #endif

 //SetMode48K();
 #ifdef use_lib_core_linkefong
  rom0 = (uint8_t *)gb_rom_0_sinclair_48k;    
  rom1 = (uint8_t *)gb_rom_0_sinclair_48k;
  rom2 = (uint8_t *)gb_rom_0_sinclair_48k;
  rom3 = (uint8_t *)gb_rom_0_sinclair_48k;
 #else
  #ifdef use_lib_core_jsanchezv
   rom0_jsanchezv = (uint8_t *)gb_rom_0_sinclair_48k;    
   rom1_jsanchezv = (uint8_t *)gb_rom_0_sinclair_48k;
   rom2_jsanchezv = (uint8_t *)gb_rom_0_sinclair_48k;
   rom3_jsanchezv = (uint8_t *)gb_rom_0_sinclair_48k;  
  #endif
 #endif 
 
 //zx48k se usa rom0 ram0, ram2 y ram5
 #ifdef use_lib_core_linkefong
  ram0 = (unsigned char *)malloc(16384);  
  #ifdef use_lib_only_48k
   ram1 = NULL;
  #else
   ram1 = (unsigned char *)malloc(16384);  
  #endif 
  ram2 = (unsigned char *)malloc(16384);
  #ifdef use_lib_only_48k   
   ram3 = NULL;
   ram4 = NULL;
  #else   
   ram3 = (unsigned char *)malloc(16384);
   ram4 = (unsigned char *)malloc(16384);  
  #endif

  #ifdef use_lib_iram_video
  #else
   ram5 = (unsigned char *)malloc(16384);
   #ifdef use_lib_only_48k
    ram7 = NULL;
   #else
    ram7 = (unsigned char *)malloc(16384);
   #endif 
  #endif
  #ifdef use_lib_only_48k
   ram6 = NULL;
  #else
   ram6 = (unsigned char *)malloc(16384);
  #endif 
  
 #else
  #ifdef use_lib_core_jsanchezv       
   //gb_ram_z80Ram_jsanchezv = (unsigned char *)malloc(0x10000);
   //gb_ram_z80Ports_jsanchezv = (unsigned char *)malloc(0x10000);
   //sim.AssignPtrRamPort(gb_ram_z80Ram_jsanchezv,gb_ram_z80Ports_jsanchezv);
   sim.runTestJJ();   
  #endif
 #endif

 //SDL_Generate_lookup_calcY();
 #ifdef use_lib_log_serial
  Serial.printf("HEAP AFTER RAM %d\r\n", ESP.getFreeHeap());
 #endif 

 #ifdef use_lib_vga320x200
  vga_init(pin_config,VgaMode_vga_mode_320x200,false); //Llamada en C
 #else
  #ifdef use_lib_vga320x240
   vga_init(pin_config,VgaMode_vga_mode_320x240,false); //Llamada en C
  #else
   #ifdef use_lib_vga360x200
    vga_init(pin_config,VgaMode_vga_mode_360x200,false); //Llamada en C
   #endif 
  #endif
 #endif 
 gb_sync_bits= vga_get_sync_bits();
 gb_buffer_vga = vga_get_framebuffer();
 gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
 PrepareColorsUltraFastVGA(); //Llamar despues de tener gb_sync_bits
 SDLClear();
 vTaskDelay(20);
//jjvga #ifdef use_lib_vga8colors
//jjvga  #ifndef use_lib_vga320x200
//jjvga   #ifdef use_lib_vga320x240
//jjvga    vga.init(vga.MODE320x240, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);        
//jjvga   #else
//jjvga    vga.init(vga.MODE360x200, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);    
//jjvga   #endif 
//jjvga  #else   
//jjvga   vga.init(vga.MODE320x200, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);       
//jjvga  #endif     
//jjvga #endif

//jjvga#ifdef COLOR_6B
//jjvga   const int redPins[] = {RED_PINS_6B};
//jjvga   const int grePins[] = {GRE_PINS_6B};
//jjvga   const int bluPins[] = {BLU_PINS_6B};
//jjvga   #ifndef use_lib_vga320x200
//jjvga    #ifdef use_lib_vga320x240
//jjvga     vga.init(vga.MODE320x240, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
//jjvga    #else   
//jjvga     vga.init(vga.MODE360x200, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
//jjvga    #endif
//jjvga   #else
//jjvga    //jjvga vga.init(vga.MODE320x200, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
//jjvga   #endif 
//jjvga#endif

//jjvga #ifdef COLOR_14B
//jjvga    const int redPins[] = {RED_PINS_14B};
//jjvga    const int grePins[] = {GRE_PINS_14B};
//jjvga    const int bluPins[] = {BLU_PINS_14B};
//jjvga    vga.init(vga.MODE360x200, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
//jjvga#endif
    
    #ifdef use_lib_log_serial
     Serial.printf("HEAP after vga  %d \r\n", ESP.getFreeHeap());
    #endif
    //jjvga vga.setFont(Font6x8);
    //jjvga vga.clear(BLACK);    
//jjvga    #ifdef use_lib_vga320x240
//jjvga     vga.clear(BLACK);
//jjvga     vga.fillRect(0,0,320,240,BLACK);
//jjvga     vga.fillRect(0,0,320,240,BLACK);//Repeat Fix visual defect   
//jjvga    #else     
//jjvga     #ifdef use_lib_vga320x200
//jjvga      //jjvga vga.clear(BLACK);
//jjvga      //jjvga vga.fillRect(0,0,320,200,BLACK);
//jjvga      //jjvga vga.fillRect(0,0,320,200,BLACK);//Repeat Fix visual defect 
//jjvga     #else      
//jjvga      #ifdef use_lib_vga360x200
//jjvga       vga.clear(BLACK);
//jjvga       vga.fillRect(0,0,360,200,BLACK);
//jjvga       vga.fillRect(0,0,360,200,BLACK);//Repeat Fix visual defect 
//jjvga      #endif
//jjvga     #endif 
//jjvga    #endif 
//jjvga    vTaskDelay(2);

    //#ifdef use_lib_core_jsanchezv
    // PreparaVGAColoresMascara();
    //#endif 

    #ifndef use_lib_resample_speaker
     pinMode(SPEAKER_PIN, OUTPUT);
     #ifdef use_lib_ultrafast_speaker
      REG_WRITE(GPIO_OUT_W1TC_REG , BIT25); //LOW clear
     #else 
      digitalWrite(SPEAKER_PIN, LOW);
     #endif 
    #endif 
    //JJ pinMode(EAR_PIN, INPUT);
    //JJ pinMode(MIC_PIN, OUTPUT);
    
    //JJ digitalWrite(MIC_PIN, LOW);

    //#ifdef use_lib_mouse_kempston
    // //gb_mouse.begin(GPIO_NUM_26, GPIO_NUM_27);
    // //PS2Controller.begin(GPIO_NUM_26, GPIO_NUM_27); // clk, dat
    // //gb_mouse.begin(0); 
    // PS2Controller.begin();
    //#endif 
    
    #ifdef use_lib_mouse_kempston   
     //mouse PS2Controller.begin();
     delay(gb_delay_init_ms); //Espera 1 segundo para detectar
     gb_mouse_init_error=0;
     pinMode(MOUSE_CLK, OUTPUT);
     pinMode(MOUSE_DATA, INPUT);
     PS2Mouse_Init(MOUSE_CLK,MOUSE_DATA); //Inicializa raton
     PS2Mouse_begin();
     gb_mouseTime=0;
    #endif 

    kb_begin();    

    //JJ Serial.printf("%s bank %u: %ub\n", MSG_FREE_HEAP_AFTER, 0, ESP.getFreeHeap());
    //JJ Serial.printf("CALC TSTATES/PERIOD %u\n", CalcTStates());

    // START Z80
    //JJ Serial.println(MSG_Z80_RESET);
    #ifdef use_lib_core_linkefong
     ReloadLocalCacheROMram(); //Recargo punteros RAM ROM
     #ifdef use_optimice_writebyte
      #ifdef use_lib_log_serial
       Serial.printf("Optimice writebyte Setup AsignarRealPtrRAM\n");
      #endif 
      AsignarRealPtrRAM();      
     #endif     
     zx_setup();     
     // make sure keyboard ports are FF
     memset((void *)z80ports_in,0x1f,32);//Optimice resize code
    #endif 
    
    #ifdef use_lib_vga_thread
     #ifdef use_lib_core_linkefong
      vidQueue = xQueueCreate(1, sizeof(uint16_t *));
      xTaskCreatePinnedToCore(&videoTask, "videoTask", 1024 * 4, NULL, 5, &videoTaskHandle, 0);
     #else
      #ifdef use_lib_core_jsanchezv
       vidQueue = xQueueCreate(1, sizeof(uint16_t *));
       xTaskCreatePinnedToCore(&videoTask_jsanchezv, "videoTask_jsanchezv", 1024 * 4, NULL, 5, &videoTaskHandle, 0);
      #endif
     #endif 
    #endif

    #ifdef use_lib_sound_ay8912
     #ifndef use_lib_resample_speaker
      for (unsigned char i=0;i<3;i++)
      {
       soundGenerator.attach(&gb_sineArray[i]);
       gb_sineArray[i].enable(true);
       gb_sineArray[i].setFrequency(0);
      }
      soundGenerator.play(true);
     #endif 
    #endif

    #ifdef use_lib_resample_speaker
     for (unsigned char i=0;i<4;i++)
     {
      soundGenerator.attach(&gb_sineArray[i]);
      gb_sineArray[i].enable(true);
      gb_sineArray[i].setFrequency(0);
     }
     soundGenerator.play(true);
    #endif 
        
    #ifdef use_lib_wifi
     Serial.printf("RAM before WIFI %d\r\n",ESP.getFreeHeap());
     //for(uint8_t t = 4; t > 0; t--) 
     //{
     //   Serial.printf("[SETUP] WAIT %d...\n", t);
     //   Serial.flush();
     //   delay(1000);
     //}
//     wifiMulti.addAP(gb_wifi_ssd, gb_wifi_pass);     

     WiFi.mode(WIFI_STA);
     //esp_wifi_set_ps(WIFI_PS_NONE); //Ahorro energia desactivado
     WiFi.begin(gb_wifi_ssd, gb_wifi_pass);

     for(unsigned char t = 4; t > 0; t--)
     {
      #ifdef use_lib_wifi_debug
       Serial.printf("WIFI WAIT %d...\r\n", t);
       Serial.flush();
      #endif
      delay(1000);
     }
     //AsignarOSD_WIFI(&wifiMulti, &http, stream);
     //AsignarOSD_WIFI(&http, stream);
     Asignar_WIFI(&http, stream);
     Check_WIFI();
     #ifdef use_lib_wifi_debug
      Serial.print("IP address:");
      Serial.println(WiFi.localIP());
     #endif 
     //AsignarDISK_WIFI(&http, stream);
    #endif 

    #ifdef use_lib_log_serial
     Serial.printf("End of setup RAM %d\r\n",ESP.getFreeHeap());
     //Serial.printf("mask %d bits %d\n",vga.RGBAXMask,vga.SBits);  
    #endif 
    gb_keyboardTime = millis();
    #ifdef use_lib_sound_ay8912
     gb_sdl_time_sound_before = gb_keyboardTime;
    #endif

    #ifdef use_lib_keyboard_uart
     gb_curTime_keyboard_before_uart= gb_curTime_keyboard_uart= gb_keyboardTime;
    #endif
}


#ifdef use_lib_sound_ay8912
 void SilenceAllChannels()
 {
  #ifdef use_lib_resample_speaker
   for (unsigned char i=0;i<4;i++)
  #endif
  #ifndef use_lib_resample_speaker
   for (unsigned char i=0;i<3;i++)
  #endif  
  {
   gb_sineArray[i].setFrequency(0);
   gb_sineArray[i].setVolume(0);
   gbVolMixer_before[i]= gbVolMixer_now[i]= 0;
   gbFrecMixer_before[i]= gbFrecMixer_now[i]= 0;
  }
  gb_ay8912_A_frec= gb_ay8912_B_frec= gb_ay8912_C_frec= 0;
  gb_ay8912_A_vol= gb_ay8912_B_vol= gb_ay8912_C_vol= 0;
 }


 inline void sound_cycleFabgl()
 {  
  //AY8912
  for (unsigned char i=0;i<3;i++)
  {
   if (gbVolMixer_now[i] != gbVolMixer_before[i])
   {
    switch (gbShiftLeftVolumen)
    {
      case 0: gb_sineArray[i].setVolume((gbVolMixer_now[i]<<2)); break;
      case 1: gb_sineArray[i].setVolume((gbVolMixer_now[i]<<1)); break;
      case 2: gb_sineArray[i].setVolume((gbVolMixer_now[i])); break;
      case 3: gb_sineArray[i].setVolume((gbVolMixer_now[i]>>1)); break;
      case 4: gb_sineArray[i].setVolume((gbVolMixer_now[i]>>2)); break;
      default: gb_sineArray[i].setVolume((gbVolMixer_now[i]<<2)); break;
    }
    //gb_sineArray[i].setVolume((gbVolMixer_now[i]<<2));
    gbVolMixer_before[i] = gbVolMixer_now[i];
   }
   if (gbFrecMixer_now[i] != gbFrecMixer_before[i])
   {
    gb_sineArray[i].setFrequency(gbFrecMixer_now[i]);
    gbFrecMixer_before[i] = gbFrecMixer_now[i];
   }
  }
 }
 
 //#define BASE_FREQ 108900
 #define BASE_FREQ 62500
 inline void jj_mixpsg() 
 {//AY8912  
  int auxFrec;
  gbVolMixer_now[0]= gb_ay8912_A_vol;
  gbVolMixer_now[1]= gb_ay8912_B_vol;
  gbVolMixer_now[2]= gb_ay8912_C_vol;
  if (gbVolMixer_now[0] == 0) gbFrecMixer_now[0] = 0;
  else{
   gbVolMixer_now[0]=15;
   auxFrec = gb_ay8912_A_frec;
   auxFrec = (auxFrec>0)?(BASE_FREQ/auxFrec):0;
   if (auxFrec>15000) auxFrec=15000;
   gbFrecMixer_now[0] = auxFrec;
  }
  if (gbVolMixer_now[1] == 0) gbFrecMixer_now[1] = 0;
  else{
   gbVolMixer_now[1]=15;
   auxFrec = gb_ay8912_B_frec;
   auxFrec = (auxFrec>0)?(BASE_FREQ/auxFrec):0;
   if (auxFrec>15000) auxFrec=15000;   
   gbFrecMixer_now[1] = auxFrec;
  }
  if (gbVolMixer_now[2] == 0) gbFrecMixer_now[2] = 0;
  else{
   gbVolMixer_now[2]=15;
   auxFrec = gb_ay8912_C_frec;
   auxFrec = (auxFrec>0)?(BASE_FREQ/auxFrec):0;
   if (auxFrec>15000) auxFrec=15000;
   gbFrecMixer_now[2] = auxFrec;
  }
  
 /*
  //AY8912  
  int auxFrec = gb_ay8912_A_frec;
  int auxVol = gb_ay8912_A_vol;

  if (auxVol != gbVolMixer_before[0]){
   gb_sineArray[0].setVolume((auxVol<<2));
   gbVolMixer_before[0] = auxVol;
  }
  if (auxFrec != gbFrecMixer_before[0]){
   gb_sineArray[0].setFrequency(auxFrec);
   gbFrecMixer_before[0] = auxFrec;
  }
  
  auxFrec = gb_ay8912_B_frec + (gb_ay8912_B_frec_course<<2); //*4
  auxVol = gb_ay8912_B_vol<<1; //*2
  if (auxVol != gbVolMixer_before[1]){
   gb_sineArray[1].setVolume(auxVol);
   gbVolMixer_before[1] = auxVol;
  }
  if (auxFrec != gbFrecMixer_before[1]){
   gb_sineArray[1].setFrequency(auxFrec);
   gbFrecMixer_before[1] = auxFrec;
  }  

  auxFrec = gb_ay8912_C_frec + (gb_ay8912_C_frec_course<<2); //*4
  auxVol = gb_ay8912_C_vol<<1; //*2
  if (auxVol != gbVolMixer_before[2]){
   gb_sineArray[2].setVolume(auxVol);
   gbVolMixer_before[2] = auxVol;
  }
  if (auxFrec != gbFrecMixer_before[2]){
   gb_sineArray[2].setFrequency(auxFrec);
   gbFrecMixer_before[2] = auxFrec;
  }
  */

  #ifdef use_lib_resample_speaker
   gbTimeNow = millis();
   if ((gbTimeNow-gbTimeIni)>=25)
   {
    gbTimeIni = gbTimeNow;
    auxFrec = gbContSPKChangeSamples<<3;
    auxVol = (gbContSPKChangeSamples != 0)? 32: 0;    
    if (auxVol != gbVolMixer_before[3]){
     gb_sineArray[3].setVolume(auxVol);
     gbVolMixer_before[3] = auxVol;
    }    
    if (auxFrec != gbFrecMixer_before[3])
    {
     gb_sineArray[3].setFrequency(auxFrec);
     gbFrecMixer_before[3] = auxFrec;
    }     
    gbContSPKChangeSamples = 0;
   }
  #endif
   
  //gb_sineArray[1].setVolume((gb_ay8912_B_vol<<1));
  //gb_sineArray[1].setFrequency((gb_ay8912_B_frec + ((gb_ay8912_B_frec_course<<2))));
  //gb_sineArray[2].setVolume((gb_ay8912_C_vol<<1));
  //gb_sineArray[2].setFrequency((gb_ay8912_C_frec + ((gb_ay8912_C_frec_course<<2))));
  

  
  /*gb_vol_array_cur[0] = (gb_ay8912_A_vol<<1);
  //if ((gb_ay8912_mixer & 0x01)==0x00)
  {      
   //if (gb_ay8912_A_vol>0)
    gb_frec_array_cur[0]= gb_ay8912_A_frec + ((gb_ay8912_A_frec_course<<2));   
  }
  //else  
  // gb_vol_array_cur[0] = 0;

  gb_vol_array_cur[1] = (gb_ay8912_B_vol<<1);
  //if ((gb_ay8912_mixer & 0x02)==0x00)
  {   
   //if (gb_ay8912_B_vol>0)
    gb_frec_array_cur[1]= gb_ay8912_B_frec + ((gb_ay8912_B_frec_course<<2));   
  }
  //else
  // gb_vol_array_cur[1] = 0;
  
    
  gb_vol_array_cur[2] = (gb_ay8912_C_vol<<1); 
  //if ((gb_ay8912_mixer & 0x04)==0x00)
  {  
   //if (gb_ay8912_B_vol>0)   
    gb_frec_array_cur[2]= gb_ay8912_C_frec  + ((gb_ay8912_C_frec_course<<2));   
  }
  //else
  // gb_vol_array_cur[2] = 0;

  //if (gb_silence==1)  
  // gb_frec_ch01_cur= gb_frec_ch02_cur= gb_frec_ch03_cur = 0;  

 // if ((gb_ay8912_mixer & 0x08) == 0x00)
 //  gb_frec_array_cur[0] = gb_frec_array_cur[0] + rand() % 20;
 // if ((gb_ay8912_mixer & 0x10) == 0x00)   
 //  gb_frec_array_cur[1] = gb_frec_array_cur[1] + rand() % 20;
 // if ((gb_ay8912_mixer & 0x20) == 0x00)
 //  gb_frec_array_cur[2] = gb_frec_array_cur[2] + rand() % 20;

  for (unsigned char i=0;i<3;i++)
  {
   gb_sineArray[i].setFrequency(gb_frec_array_cur[i]);   
   gb_sineArray[i].setVolume(gb_vol_array_cur[i]);
  }   
  */
 }
#endif 

#ifdef use_lib_mouse_kempston
void PollMouse(void)
{
 unsigned char stat,auxSwap;
 signed short int x,y;
 PS2Mouse_getPosition(stat,x,y);
 gb_mouse_key_right = gb_mouse_key_left= gb_mouse_key_down = gb_mouse_key_up = 1;

 //Botones
 gb_mouse_key_btn_left = ((stat & 0x01) == 0x01)?0:1;
 gb_mouse_key_btn_right = ((stat & 0x02) == 0x02)?0:1;
 gb_mouse_key_btn_middle = ((stat & 0x04) == 0x04)?0:1;

 if (gb_force_left_handed==1){
  auxSwap= gb_mouse_key_btn_right; //zurdo activo
  gb_mouse_key_btn_right= gb_mouse_key_btn_left;
  gb_mouse_key_btn_left= auxSwap;
 }

 //Delta
 if (gb_mouse_invert_deltax == 0){
  gb_mouse_key_right= (x > 0)?0:1;
  gb_mouse_key_left = (x < 0)?0:1;
 }
 else{
  gb_mouse_key_right= (x < 0)?0:1;
  gb_mouse_key_left = (x > 0)?0:1;
 }
 
 if (gb_mouse_invert_deltay == 0){     
  gb_mouse_key_down = (y < 0)?0:1;
  gb_mouse_key_up = (y > 0 )?0:1;
 }
 else{
  gb_mouse_key_down = (y > 0)?0:1;
  gb_mouse_key_up = (y < 0 )?0:1;     
 }


 gb_z80_mouseBtn = 0x00;
 if (gb_mouse_key_btn_right == 0)
   gb_z80_mouseBtn = gb_z80_mouseBtn | 0x01;
 if (gb_mouse_key_btn_left == 0)
 {
  gb_z80_mouseBtn = gb_z80_mouseBtn | 0x02;
  //keymap[KEY_ALT_GR] = 0;
 }
 //else
 // keymap[KEY_ALT_GR] = 1;
 if (gb_mouse_key_btn_middle == 0)   
  gb_z80_mouseBtn = gb_z80_mouseBtn | 0x04;
 gb_z80_mouseBtn = (~gb_z80_mouseBtn)|0x08; //KMouse protocol always 1 bit3 

 if (x>0){
  if (gb_mouse_invert_deltax == 0){
   gb_z80_mouse_x += (int)gb_mouse_inc;
  }
  else{
   gb_z80_mouse_x -= (int)gb_mouse_inc;
  }
 }
 if (x<0){
  if (gb_mouse_invert_deltax == 0){
   gb_z80_mouse_x -= (int)gb_mouse_inc;
  }
  else{
   gb_z80_mouse_x += (int)gb_mouse_inc;
  }
 }

 if (y>0){
  if (gb_mouse_invert_deltay == 0){
   gb_z80_mouse_y += (int)gb_mouse_inc;
  }
  else{
   gb_z80_mouse_y -= (int)gb_mouse_inc;
  }
 }
 if (y<0){
  if (gb_mouse_invert_deltay == 0){
   gb_z80_mouse_y -= (int)gb_mouse_inc;
  }
  else{
   gb_z80_mouse_y += (int)gb_mouse_inc;
  }
 }


 #ifdef use_lib_core_linkefong
  //El Joystick es logica inversa 1 es boton pulsado
  if (gb_mouse_key_btn_left==1){
   bitWrite(z80ports_in[0x1f], 4, 1); //KEY_ALT_GR
  }
  if (x > 0){
   bitWrite(z80ports_in[0x1f], 0, 1);  //KEY_CURSOR_RIGHT
  }
  if (x < 0){
   bitWrite(z80ports_in[0x1f], 1, 1); //KEY_CURSOR_LEFT
  }
  if (y<0){
   bitWrite(z80ports_in[0x1f], 2, 1); //KEY_CURSOR_DOWN
  }
  if (y>0){
   bitWrite(z80ports_in[0x1f], 3, 1); //KEY_CURSOR_UP
  }
 #else
  #ifdef use_lib_core_jsanchezv
   if (gb_mouse_key_btn_left==1){
    bitWrite(z80Ports_jsanchezv[0x1f], 4, 1); //KEY_ALT_GR
   }
   if (x > 0){
    bitWrite(z80Ports_jsanchezv[0x1f], 0, 1);  //KEY_CURSOR_RIGHT
   }
   if (x < 0){
    bitWrite(z80Ports_jsanchezv[0x1f], 1, 1); //KEY_CURSOR_LEFT
   }
   if (y<0){
    bitWrite(z80Ports_jsanchezv[0x1f], 2, 1); //KEY_CURSOR_DOWN
   }
   if (y>0){
    bitWrite(z80Ports_jsanchezv[0x1f], 3, 1); //KEY_CURSOR_UP
   }    
  #endif
 #endif
 
 //Serial.printf("X:%d Y:%d mX:%d mY:%d\n",x,y,gb_z80_mouse_x,gb_z80_mouse_y);

  //keymap[KEY_CURSOR_RIGHT] = (x > 0)?0:1;       
  //keymap[KEY_CURSOR_LEFT] = (x < 0)?0:1;
  //keymap[KEY_CURSOR_DOWN] = (y < 0)?0:1;
  //keymap[KEY_CURSOR_UP] = (y > 0 )?0:1;  

/*
 auto mouse = PS2Controller.mouse();
 if (mouse->deltaAvailable())
 {
  MouseDelta mouseDelta;
  mouse->getNextDelta(&mouseDelta);
  gb_z80_mouseBtn = 0x00;
  if (mouseDelta.buttons.right)
   gb_z80_mouseBtn = gb_z80_mouseBtn | 0x01;
  if (mouseDelta.buttons.left)
  {
   gb_z80_mouseBtn = gb_z80_mouseBtn | 0x02;
   keymap[KEY_ALT_GR] = 0;
  }
  else
   keymap[KEY_ALT_GR] = 1;
  if (mouseDelta.buttons.middle)   
   gb_z80_mouseBtn = gb_z80_mouseBtn | 0x04;
  gb_z80_mouseBtn = (~gb_z80_mouseBtn)|0x08; //KMouse protocol always 1 bit3

  gb_z80_mouse_x = gb_z80_mouse_x + mouseDelta.deltaX;
  gb_z80_mouse_y = gb_z80_mouse_y + mouseDelta.deltaY;

  keymap[KEY_CURSOR_RIGHT] = (mouseDelta.deltaX > 0)?0:1;       
  keymap[KEY_CURSOR_LEFT] = (mouseDelta.deltaX < 0)?0:1;
  keymap[KEY_CURSOR_DOWN] = (mouseDelta.deltaY < 0)?0:1;
  keymap[KEY_CURSOR_UP] = (mouseDelta.deltaY > 0 )?0:1; 
   
  
  //Serial.printf("deltaX = %d  deltaY = %d  deltaZ = %d  leftBtn = %d  midBtn = %d  rightBtn = %d\n",
  //              mouseDelta.deltaX, mouseDelta.deltaY, mouseDelta.deltaZ,
  //              mouseDelta.buttons.left, mouseDelta.buttons.middle, mouseDelta.buttons.right);
 } 
 */
}
#endif

// VIDEO core 0 *************************************

/*
//Medio optimizado
void videoTask(void *unused)
{
    unsigned long time_prev;
    unsigned int ff, i, byte_offset;
    unsigned char color_attrib, pixel_map, flash, bright;
    unsigned int zx_vidcalc, calc_y;

    word zx_fore_color, zx_back_color, tmp_color;
    // byte active_latch;

    videoTaskIsRunning = true;
    uint16_t *param;
    //int auxY;
    //unsigned char auxColor;

    while (1) 
    {        
        xQueuePeek(vidQueue, &param, portMAX_DELAY);
        if ((int)param == 1)
            break;

        time_prev = micros();   

        for (unsigned int vga_lin = 0; vga_lin < 200; vga_lin++) {
          //for (unsigned int vga_lin = 0; vga_lin < 150; vga_lin++) {
            // tick = 0;
            if (vga_lin < 3 || vga_lin > 194) {
            //if (vga_lin < 3 || vga_lin > 144) {
                //auxY= vga_lin^2;
                //auxColor= (gb_cache_zxcolor[borderTemp] & 0x3f) | 0xc0;
                for (int bor = 32; bor < 328; bor++)
                {
                //for (int bor = 32; bor < 320; bor++) //360-328=32
                //for (int bor = 32; bor < 188; bor++) //360-328=32
                    //JJvga.dotFast(bor, vga_lin, zxcolor(borderTemp, 0));
                 vga.dotFast(bor, vga_lin, gb_cache_zxcolor[borderTemp]);
                 //vga.backBuffer[auxY][bor] = auxColor;
                }
            } else {

                for (int bor = 32; bor < 52; bor++)
                {
                    //JJvga.dotFast(bor, vga_lin, zxcolor(borderTemp, 0));
                    //JJvga.dotFast(bor + 276, vga_lin, zxcolor(borderTemp, 0));
                    vga.dotFast(bor, vga_lin, gb_cache_zxcolor[borderTemp]);
                    vga.dotFast(bor + 276, vga_lin, gb_cache_zxcolor[borderTemp]);

                    //vga.dotFast(bor + 268, vga_lin, zxcolor(borderTemp, 0));
                    //vga.dotFast(bor + 140, vga_lin, zxcolor(borderTemp, 0));
                }

                for (ff = 0; ff < 32; ff++) // foreach byte in line
                {
                    byte_offset = (vga_lin - 3) * 32 + ff;
                    calc_y = calcY(byte_offset);
                    if (!video_latch)
                    {
                       color_attrib = readbyte(0x5800 + (calc_y / 8) * 32 + ff); // get 1 of 768 attrib values
                       pixel_map = readbyte(byte_offset + 0x4000);
                     } else
                      {
                       color_attrib = ram7[0x1800 + (calc_y / 8) * 32 + ff]; // get 1 of 768 attrib values                        
                       pixel_map = ram7[byte_offset];//JJ                        
                      }

                    for (i = 0; i < 8; i++) // foreach pixel within a byte
                    {

                        zx_vidcalc = ff * 8 + i;
                        byte bitpos = (0x80 >> i);
                        bright = (color_attrib & 0B01000000) >> 6;
                        flash = (color_attrib & 0B10000000) >> 7;
                        //JJ zx_fore_color = zxcolor((color_attrib & 0B00000111), bright);
                        //JJ zx_back_color = zxcolor(((color_attrib & 0B00111000) >> 3), bright);
                        zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                        zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo

                        if (flash && flashing)
                            swap_flash(&zx_fore_color, &zx_back_color);

                        if ((pixel_map & bitpos) != 0)
                        {
                          //if (zx_vidcalc < 268){
                            vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_fore_color);
                          //}
                        }
                        else
                        {
                          //if (zx_vidcalc < 268){
                            vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_back_color);
                          //}
                        }
                    }
                }
            }
        }

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
        videoTaskIsRunning = false;

     time_prev = micros()-time_prev;
     Serial.printf("Tiempo %d\r\n",time_prev);             
    }
    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {
    }
}
*/



/*
//10500 micros
//Video task optimize
void videoTask(void *unused)
{
    unsigned long time_prev;
    
    unsigned int ff, i, byte_offset;
    unsigned char color_attrib, pixel_map, flash, bright;
    unsigned int zx_vidcalc, calc_y;    

    word zx_fore_color, zx_back_color, tmp_color;    

    videoTaskIsRunning = true;
    uint16_t *param;    
    unsigned char auxColor;
    #define gbvgaMask8Colores 0x3F
    #define gbvgaBits8Colores 0x40
    
    while (1) 
    {        
        xQueuePeek(vidQueue, &param, portMAX_DELAY);
        if ((int)param == 1)
            break;

        time_prev = micros();
        
        for (unsigned int vga_lin = 0; vga_lin < 200; vga_lin++)        
        {
            auxColor = (gb_local_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores;
            if (vga_lin < 3 || vga_lin > 194)
            {
                for (int bor = 32; bor < 328; bor++){
                 vga.backBuffer[vga_lin][bor^2] = auxColor;
                }
            }
            else            
            {
                for (int bor = 32; bor < 52; bor++)
                {
                    vga.backBuffer[vga_lin][bor^2] = auxColor;
                    vga.backBuffer[vga_lin][(bor+276)^2] = auxColor;
                }
                for (ff = 0; ff < 32; ff++) // foreach byte in line
                {
                    byte_offset = (vga_lin - 3) * 32 + ff;
                    calc_y = calcY(byte_offset);
                    if (!video_latch)
                    {
                       color_attrib = local_fast_readbyte(0x5800 + (calc_y / 8) * 32 + ff); // get 1 of 768 attrib values                       
                       pixel_map = local_fast_readbyte(byte_offset + 0x4000);
                     } else
                      {
                       color_attrib = ram7[0x1800 + (calc_y / 8) * 32 + ff]; // get 1 of 768 attrib values                       
                       pixel_map = ram7[byte_offset];//JJ                        
                      }

                    for (i = 0; i < 8; i++) // foreach pixel within a byte
                    {
                        zx_vidcalc = ff * 8 + i;
                        byte bitpos = (0x80 >> i);
                        bright = (color_attrib & 0B01000000) >> 6;
                        flash = (color_attrib & 0B10000000) >> 7;
                        zx_fore_color = gb_local_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                        zx_back_color = gb_local_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo

                        if (flash && flashing)
                            swap_flash(&zx_fore_color, &zx_back_color);

                        if ((pixel_map & bitpos) != 0)
                        {   
                            auxColor = (zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores;
                            vga.backBuffer[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                            //vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_fore_color);
                        }
                        else
                        {                          
                            auxColor = (zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores;
                            vga.backBuffer[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                            //vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_back_color);                          
                        }
                    }
                }
            }
        }

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
        videoTaskIsRunning = false;
     time_prev = micros()-time_prev;
     Serial.printf("Tiempo %d\r\n",time_prev);     
    }
    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {
    }
}
*/

//void SDL_Generate_lookup_calcY()
//{//200*32=6400
// for (int i=0;i<6400;i++)
// {  
//  gb_lookup_calcY[i]= calcY(i);
// }
//}

/* El bueno con hilos
#ifdef use_lib_core_linkefong
 #ifdef use_lib_vga_thread
//Video con hilos
void videoTask(void *unused)
{
    //8300 microsegundos
    //unsigned long time_prev;    
    int byte_offset;
    unsigned char color_attrib, flash;
    //int pixel_map;//, bright;
    int zx_vidcalc, calc_y;
    #ifdef use_lib_ultrafast_vga
     unsigned char ** ptrVGA;
    #endif

    word zx_fore_color, zx_back_color, tmp_color;    

    unsigned char auxColor;
    unsigned char * gb_ptr_ram_1800_video;
    unsigned char * gb_ptr_ram_video;
    unsigned char bitpos;
    #ifdef use_lib_skip_frame
     unsigned char skipFrame=0;
    #endif

    videoTaskIsRunning = true;
    uint16_t *param;
    #ifdef use_lib_ultrafast_vga
     #define gbvgaMask8Colores 0x3F
     #define gbvgaBits8Colores 0x40
     ptrVGA = vga.backBuffer;
    #endif 
    
    while (1) 
    {        
        xQueuePeek(vidQueue, &param, portMAX_DELAY);
        if ((int)param == 1)
            break;

#ifdef use_lib_skip_frame
if (
    (gbFrameSkipVideoCurrentCont < gbFrameSkipVideoMaxCont)
    && 
    (gbFrameSkipVideoMaxCont != 0)
   )
{
 gbFrameSkipVideoCurrentCont++; 
 skipFrame = 1;
}
else
{ 
 gbFrameSkipVideoCurrentCont= 0;
 skipFrame = 0;
}
#endif

#ifdef use_lib_skip_frame
if (skipFrame != 1)
#endif
{

    if (!video_latch){
     gb_ptr_ram_1800_video = &ram5[0x1800];
     gb_ptr_ram_video = ram5;
    }
    else{
     gb_ptr_ram_1800_video = &ram7[0x1800];
     gb_ptr_ram_video = ram7;  
    }    

        //time_prev = micros();     
        for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
        {
            #ifdef use_lib_ultrafast_vga             
             #ifdef use_lib_vga_low_memory
              //Modo bajo y rapido
              #ifdef use_lib_vga8colors
               auxColor = (gb_cache_zxcolor[borderTemp]); //8 colores seguro bajo
              #else
               auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores; //Modo bajo
              #endif 
             #else
              //Modo alto y rapido 
              auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores; //Modo bajo
             #endif 
            #else
             //Modo lento
             auxColor = (gb_cache_zxcolor[borderTemp]);
            #endif
            
            if (vga_lin < 3 || vga_lin > 194)
            {
                #ifdef use_lib_screen_offset
                if ((gb_screen_yIni+vga_lin)>=0)
                #endif
                {                                    
                 #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)                    
                  for (int bor = 0; bor < 296; bor++)
                 #else
                  for (int bor = 32; bor < 328; bor++)
                 #endif
                  {                      
                    //vga.backBuffer[vga_lin][bor^2] = auxColor;
                    //vga.backBuffer[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                    #ifdef use_lib_ultrafast_vga
                     #ifdef use_lib_vga_low_memory
                      //Modo bajo y rapido
                      #ifdef use_lib_vga8colors
                       vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor); //8 colores seguro bajo
                      #else
                       #ifdef use_lib_screen_offset
                        ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)] = auxColor;
                       #else
                        ptrVGA[vga_lin][bor] = auxColor;                      
                       #endif
                      #endif 
                     #else
                      //Modo alto y rapido 
                      #ifdef use_lib_screen_offset
                       ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                      #else
                       ptrVGA[vga_lin][bor^2] = auxColor;
                      #endif
                     #endif
                    #else
                     //Modo lento  
                     #ifdef use_lib_screen_offset
                      vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                     #else
                      vga.dotFast(bor,vga_lin,auxColor);
                     #endif
                    #endif 


//                    #ifdef use_lib_vga_low_memory
//                     vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
//                    #else
//                     #ifdef use_lib_screen_offset
//                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
//                     #else
//                      ptrVGA[vga_lin][bor^2] = auxColor;
//                     #endif
//                    #endif
//                    vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                    
                  }
                }
            }
            else            
            {
               #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)
                for (int bor = 0; bor < 20; bor++)
               #else
                for (int bor = 32; bor < 52; bor++)
               #endif 
                {
                    //vga.backBuffer[vga_lin][bor^2] = auxColor;
                    //vga.backBuffer[vga_lin][(bor+276)^2] = auxColor;
                    //vga.backBuffer[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                    //vga.backBuffer[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)^2] = auxColor;
                  #ifdef use_lib_ultrafast_vga
                   #ifdef use_lib_vga_low_memory
                    //Modo bajo y rapido
                    #ifdef use_lib_vga8colors
                     vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                     vga.dotFast((gb_screen_xIni+bor+276),(gb_screen_yIni+vga_lin),auxColor);                    
                    #else
                     #ifdef use_lib_screen_offset
                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)] = auxColor;
                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)] = auxColor;
                     #else
                      ptrVGA[vga_lin][bor] = auxColor;
                      ptrVGA[vga_lin][(bor+276)] = auxColor;
                     #endif
                    #endif
                   #else
                    //Modo alto y rapido
                    #ifdef use_lib_screen_offset
                     ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                     ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)^2] = auxColor;
                    #else
                     ptrVGA[vga_lin][bor^2] = auxColor;
                     ptrVGA[vga_lin][(bor+276)^2] = auxColor;
                    #endif                    
                   #endif
                  #else
                   //Modo lento
                   #ifdef use_lib_screen_offset
                    vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                    vga.dotFast((gb_screen_xIni+bor+276),(gb_screen_yIni+vga_lin),auxColor);
                   #else
                    vga.dotFast(bor,vga_lin,auxColor);
                    vga.dotFast((bor+276),vga_lin,auxColor);
                   #endif
                  #endif



//                    #ifdef use_lib_vga_low_memory
//                     vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
//                     vga.dotFast((gb_screen_xIni+bor+276),(gb_screen_yIni+vga_lin),auxColor);
//                    #else
//                     #ifdef use_lib_screen_offset
//                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
//                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)^2] = auxColor;
//                     #else
//                      ptrVGA[vga_lin][bor^2] = auxColor;
//                      ptrVGA[vga_lin][(bor+276)^2] = auxColor;
//                     #endif
//                    #endif                    
//                   vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                   
                }
                byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
                for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
                {                    
                    //calc_y = calcY(byte_offset);                    
                    calc_y = gb_lookup_calcY[(byte_offset>>5)];
                    color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
                    zx_vidcalc = (ff << 3);
                    bitpos = 0x80;
                    for (unsigned char i = 0; i < 8; i++) // foreach pixel within a byte
                    {
                        flash = (color_attrib & 0B10000000) >> 7;
                        #ifdef use_lib_use_bright
                         if (((color_attrib & 0B01000000) >> 6) == 0x01)
                         {
                          zx_fore_color = gb_cache_zxcolor_bright[(color_attrib & 0B00000111)]; //con brillo
                          zx_back_color = gb_cache_zxcolor_bright[((color_attrib & 0B00111000) >> 3)];//con brillo                             
                         }
                         else
                         {
                          zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //sin brillo
                          zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//sin brillo
                         }
                        #else
                         zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                         zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
                        #endif
                        if (flash && flashing)
                        {
                         swap_flash(&zx_fore_color, &zx_back_color);
                        }
                        #ifdef use_lib_ultrafast_vga
                         #ifdef use_lib_vga_low_memory
                          //Modo bajo y rapido
                          #ifdef use_lib_vga8colors
                           auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color; //8 colores seguro bajo
                           #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                            vga.dotFast((gb_screen_xIni+zx_vidcalc + 20),(gb_screen_yIni+(calc_y + 3)),auxColor);
                           #else
                            vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);
                           #endif 
                          #else
                           auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores); //Modo bajo
                           #ifdef use_lib_screen_offset                         
                            #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                             ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 20)] = auxColor;
                            #else
                             ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)] = auxColor;
                            #endif
                           #else
                            #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                             ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)] = auxColor;
                            #else
                             ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)] = auxColor;
                            #endif
                           #endif
                          #endif
                         #else 
                          //Modo alto y rapido
                          auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores);
                          #ifdef use_lib_screen_offset
                           #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                            ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 20)^2] = auxColor;
                           #else
                            ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)^2] = auxColor;
                           #endif 
                          #else                          
                           #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)^2] = auxColor;
                           #else
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                           #endif 
                          #endif
                         #endif
                        #else
                         //Modo lento
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color; //8 colores seguro bajo
                         #ifdef use_lib_screen_offset
                          #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                           vga.dotFast((gb_screen_xIni+zx_vidcalc + 20),(gb_screen_yIni+(calc_y + 3)),auxColor);
                          #else
                           vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);
                          #endif
                         #else
                          #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                           vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                          #else
                           vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor);
                          #endif 
                         #endif
                        #endif                         
                           
//                        #ifdef use_lib_vga_low_memory
//                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color;
//                        #else
//                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores);
//                        #endif
//                        //vga.backBuffer[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
//                        //vga.backBuffer[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)^2] = auxColor;
//                        #ifdef use_lib_vga_low_memory
//                         vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);
//                        #else                         
//                         #ifdef use_lib_screen_offset                         
//                          ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)^2] = auxColor;
//                         #else
//                         #endif
//                          ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
//                        #endif
//                        auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color;
//                        vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);
                        

                        zx_vidcalc++;
                        bitpos = (bitpos>>1);
                    }
                    byte_offset++;
                }
            }
        }
}//fin if skipframe

//gb_sdl_blit=1;

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
        videoTaskIsRunning = false;
     //time_prev = micros()-time_prev;
     //Serial.printf("Tiempo %d\n",time_prev);     
    }
    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {
    }    

}

#else
*/
//Video sin hilos
/*
void videoTaskNoThread()
{
    //8300 microsegundos
    //7680 microsegundos quitando desplazamiento y skip frame
    unsigned int a0,a1,a2,a3,a32,a32_0,a32_1;
    unsigned long time_prev;    
    int byte_offset;
    unsigned char color_attrib, flash;
    //int pixel_map;//, bright;
    int zx_vidcalc, calc_y;    

    word zx_fore_color, zx_back_color, tmp_color;    

    unsigned char auxColor;
    unsigned char a[8];
    unsigned char * gb_ptr_ram_1800_video;
    unsigned char * gb_ptr_ram_video;
    unsigned char bitpos;
    #ifdef use_lib_ultrafast_vga
     unsigned char ** ptrVGA;
     #define gbvgaMask8Colores 0x3F
     #define gbvgaBits8Colores 0x40
     //jjvga ptrVGA = vga.backBuffer;
     ptrVGA= gb_buffer_vga;
    #endif

    if (!video_latch){
     gb_ptr_ram_1800_video = &ram5[0x1800];
     gb_ptr_ram_video = ram5;
    }
    else{
     gb_ptr_ram_1800_video = &ram7[0x1800];
     gb_ptr_ram_video = ram7;  
    }    

        time_prev = micros();     
        for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
        {            
            #ifdef use_lib_ultrafast_vga
             #ifdef use_lib_vga_low_memory
              //Modo bajo y rapido
              #ifdef use_lib_vga8colors
               auxColor = (gb_cache_zxcolor[borderTemp]); //8 colores seguro bajo
              #else               
               auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores; //Modo bajo
              #endif 
             #else
              //Modo alto y rapido 
              //jjvga auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores;
              auxColor = gb_cache_zxcolor[borderTemp];
             #endif 
            #else
             //Modo lento
             auxColor = (gb_cache_zxcolor[borderTemp]);
            #endif
                
            if (vga_lin < 3 || vga_lin > 194)
            {
                #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)                 
                 //for (int bor = 0; bor < 296; bor++)
                 for (int bor = 0; bor < 74; bor++) //296 DIV 4 32 bits
                #else 
                 for (int bor = 32; bor < 328; bor++)                
                #endif
                {                    
                  #ifdef use_lib_ultrafast_vga
                   #ifdef use_lib_vga_low_memory
                    //Modo bajo y rapido
                    #ifdef use_lib_vga8colors
                     vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                    #else 
                     ptrVGA[vga_lin][bor] = auxColor; //Modo bajo
                    #endif 
                   #else
                    //Modo alto y rapido
                    a0= auxColor;
                    //a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
                    a0= a0 | (a0<<8) | (a0<<16) | (a0<<24);
                    //ptrVGA[vga_lin][bor^2] = auxColor;
                    gb_buffer_vga32[vga_lin][bor] = a0;
                   #endif
                  #else
                   //Modo lento                   
                   vga.dotFast(bor,vga_lin,auxColor);
                  #endif
                }                
            }
            else            
            {
                #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)                 
                 //for (int bor = 0; bor < 20; bor++)
                 for (int bor = 0; bor < 5; bor++) //20 DIV 4 32 bits
                #else
                 for (int bor = 32; bor < 52; bor++)
                #endif               
                {                    
                 #ifdef use_lib_ultrafast_vga
                  #ifdef use_lib_vga_low_memory
                   //Modo bajo y rapido
                   #ifdef use_lib_vga8colors
                    vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                    vga.dotFast((bor+276),vga_lin,auxColor);
                   #else 
                    ptrVGA[vga_lin][bor] = auxColor; //Modo bajo
                    ptrVGA[vga_lin][(bor+276)] = auxColor;
                   #endif
                  #else
                   //Modo alto y rapido
                   //ptrVGA[vga_lin][bor^2] = auxColor;
                   //ptrVGA[vga_lin][(bor+276)^2] = auxColor;
                   a0= auxColor;
                   a0= a0 | (a0<<8) | (a0<<16) | (a0<<24);
                   gb_buffer_vga32[vga_lin][bor] = a0;
                   gb_buffer_vga32[vga_lin][bor+69] = a0;
                  #endif
                 #else
                  //Modo lento
                  vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                  vga.dotFast((bor+276),vga_lin,auxColor);
                 #endif
                }
                byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
                for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
                {                    
                    //calc_y = calcY(byte_offset);                    
                    calc_y = gb_lookup_calcY[(byte_offset>>5)];
                    color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
                    //zx_vidcalc = (ff << 3); //DIV 4 32 bits
//                    zx_vidcalc= (ff<<3) >> 2;  //Ya no lo necesito
                    bitpos = 0x80;
                    for (unsigned char i = 0; i < 8; i++) // foreach pixel within a byte
                    //for (unsigned char i = 0; i < 2; i++) //DIV 4 32 bits
                    {
                        flash = (color_attrib & 0B10000000) >> 7;
                        zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                        zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
                        if (flash && flashing)
                        {
                         swap_flash(&zx_fore_color, &zx_back_color);
                        }
                        #ifdef use_lib_ultrafast_vga
                         //jjvga auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores);
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color;
                        #else
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color : zx_back_color;
                        #endif

                        
                        #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                         //320x240
                         #ifdef use_lib_ultrafast_vga
                          #ifdef use_lib_vga_low_memory
                           //Modo bajo y rapido
                           #ifdef use_lib_vga8colors
                            vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                           #else 
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)] = auxColor;
                           #endif
                          #else
                           //Modo alto y rapido  
                           //ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)^2] = auxColor;
                           //a0 = auxColor;
                           //a0 = a0 | (a0<<8) | (a0<<16) | (a0<<24);                           
                           //gb_buffer_vga32[(calc_y + 3)][(zx_vidcalc + 5)] = auxColor;
                           a[i]= auxColor;
                          #endif                            
                         #else
                          //Modo lento
                          vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                         #endif                         
                        #else
                         //360x200
                         #ifdef use_lib_ultrafast_vga
                          #ifdef use_lib_vga_low_memory
                           //Modo bajo y rapido
                           #ifdef use_lib_vga8colors
                            vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor); //8 colores seguro bajo
                           #else
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)] = auxColor;  //Modo bajo
                           #endif
                          #else
                           //Modo alto y rapido
                           ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                          #endif                           
                         #else
                          //Modo lento 
                          vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor);
                         #endif
                        #endif 

//                        zx_vidcalc++; //DIV 4 32 bits ya no lo necesito
                        //zx_vidcalc+=4;
                        bitpos = (bitpos>>1);
                    }
                    
                    //zx_vidcalc= (ff<<3) >> 2; //Lo saco fuera 32 bits
                    zx_vidcalc= (ff<<1); //Lo saco fuera 32 bits
                    //a0= a[0];
                    //a1= a[1];
                    //a2= a[2];
                    //a3= a[3];
                    a32_0= a[2] | (a[3]<<8) | (a[0]<<16) | (a[1]<<24);
                    a32_1= a[6] | (a[7]<<8) | (a[4]<<16) | (a[5]<<24);
                    gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+5] = a32_0;                    
                    //a0= a[4];
                    //a1= a[5];
                    //a2= a[6];
                    //a3= a[7];                    
                    gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+6] = a32_1;

                    byte_offset++;
                }
            }
        }
        //gb_sdl_blit=1;
     time_prev = micros()-time_prev;
     #ifdef use_lib_log_serial
      Serial.printf("Tiempo %d\n",time_prev);             
     #endif
}
*/



//Con hilos
#ifdef use_lib_core_linkefong
 #ifdef use_lib_vga_thread
  void videoTask(void *unused)
  {
   unsigned char paleta[2]; //0 backcolor 1 Forecolor
   unsigned int a0,a1,a2,a3,a32;

   #ifdef use_lib_stats_time_unified
    unsigned long time_prev;
   #endif
   int byte_offset;
   unsigned char color_attrib, flash;
   //int pixel_map;//, bright;
   int zx_vidcalc, calc_y;    

   //word zx_fore_color, zx_back_color, tmp_color;
   unsigned char zx_fore_color, zx_back_color;
   //, tmp_color; //No se usa

   unsigned char auxColor;    
   unsigned char * gb_ptr_ram_1800_video;
   unsigned char * gb_ptr_ram_video;
   //unsigned char bitpos;

   #ifdef use_lib_skip_frame
    volatile unsigned char skipFrame=0;
   #endif
   videoTaskIsRunning = true;
   uint16_t *param;

  while (1) 
  {        
   xQueuePeek(vidQueue, &param, portMAX_DELAY);
   if ((int)param == 1){
    break;
   }
   #ifdef use_lib_skip_frame
    if(
       (gbFrameSkipVideoCurrentCont < gbFrameSkipVideoMaxCont)
       && 
       (gbFrameSkipVideoMaxCont != 0)
      )
    {
     gbFrameSkipVideoCurrentCont++; 
     skipFrame = 1;
    }
    else
    { 
     gbFrameSkipVideoCurrentCont= 0;
     skipFrame = 0;
    }
   #endif


  #ifdef use_lib_skip_frame
  if ((skipFrame != 1) && (gb_draw_thread == 1))
  #else
  if (gb_draw_thread == 1)
  #endif
  {

   if (!video_latch)
   {
    gb_ptr_ram_1800_video = &ram5[0x1800];
    gb_ptr_ram_video = ram5;
   }
   else
   {
    gb_ptr_ram_1800_video = &ram7[0x1800];
    gb_ptr_ram_video = ram7;  
   }    

   #ifdef use_lib_stats_time_unified
    time_prev = micros();
   #endif
   for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
   {     
    auxColor = gb_cache_zxcolor[borderTemp];
    a0= auxColor;
    a32= a0 | (a0<<8) | (a0<<16) | (a0<<24);
    if (vga_lin < 3 || vga_lin > 194)
    {
     #ifdef use_lib_screen_offset
      if ((gb_screen_yIni+vga_lin)<0)
      {
       continue;
      }
     #endif

     #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)
      for (int bor = 0; bor < 74; bor++) //296 DIV 4 32 bits
     #else
      for (int bor = 8; bor < 82; bor++) //32 DIV 4   328 DIV 4 32 bits
     #endif   
     {                    
      //ptrVGA[vga_lin][bor^2] = auxColor;
      #ifdef use_lib_screen_offset
       gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
      #else
       gb_buffer_vga32[vga_lin][bor] = a32;
      #endif
     }                
    }
    else            
    {
     #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)
      for (int bor = 0; bor < 5; bor++) //20 DIV 4 32 bits
     #else
      for (int bor = 8; bor < 13; bor++) //32 DIV 4   52 DIV 4 32 bits
     #endif   
     {                    
       //ptrVGA[vga_lin][bor^2] = auxColor;
       //ptrVGA[vga_lin][(bor+276)^2] = auxColor;
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
        gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+69+gb_screen_xIni] = a32;       
       #else
        gb_buffer_vga32[vga_lin][bor] = a32;
        gb_buffer_vga32[vga_lin][bor+69] = a32;
       #endif 
     }
     byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
          
     for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
     {                    
      //calc_y = calcY(byte_offset);                    
      calc_y = gb_lookup_calcY[(byte_offset>>5)];
      color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
      //zx_vidcalc = (ff << 3); //DIV 4 32 bits
      //zx_vidcalc= (ff<<3) >> 2;  //Ya no lo necesito

      //BEGIN El color lo saco fuera 8 pixels
      flash = (color_attrib & 0B10000000) >> 7;
      #ifdef use_lib_use_bright
       if (((color_attrib & 0B01000000) >> 6) == 0x01)
       {
        zx_fore_color = gb_cache_zxcolor_bright[(color_attrib & 0B00000111)]; //con brillo
        zx_back_color = gb_cache_zxcolor_bright[((color_attrib & 0B00111000) >> 3)];//con brillo                             
       }
       else
       {
        zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //sin brillo
        zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//sin brillo
       }                    
      #else
       zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
       zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
      #endif 
      if (flash && flashing)
      {
       swap_flash(&zx_fore_color, &zx_back_color);
      }
                    
      paleta[0]= zx_back_color;
      paleta[1]= zx_fore_color;
      
      zx_vidcalc= (ff<<1); //Lo saco fuera 32 bits

      a0= paleta[((gb_ptr_ram_video[byte_offset] >>7) & 0x01)];
      a1= paleta[((gb_ptr_ram_video[byte_offset] >>6) & 0x01)];
      a2= paleta[((gb_ptr_ram_video[byte_offset] >>5) & 0x01)];
      a3= paleta[((gb_ptr_ram_video[byte_offset] >>4) & 0x01)];
                     
      a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
      #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+5+gb_screen_xIni] = a32; //20 DIV 4 = 5
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+5] = a32; //20 DIV 4 = 5
       #endif
      #else
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+13+gb_screen_xIni] = a32; //52 DIV 4 = 13
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+13] = a32; //52 DIV 4 = 13
       #endif 
      #endif 

      a0= paleta[((gb_ptr_ram_video[byte_offset] >>3) & 0x01)];
      a1= paleta[((gb_ptr_ram_video[byte_offset] >>2) & 0x01)];
      a2= paleta[((gb_ptr_ram_video[byte_offset] >>1) & 0x01)];
      a3= paleta[((gb_ptr_ram_video[byte_offset]) & 0x01)];
                     
      a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
      #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+6+gb_screen_xIni] = a32; //20 DIV 4 = 5
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+6] = a32; //20 DIV 4 = 5
       #endif
      #else
       #ifdef use_lib_screen_offset
        gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+14+gb_screen_xIni] = a32; //52 DIV 4 = 13
       #else
        gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+14] = a32; //52 DIV 4 = 13
       #endif 
      #endif 
                

      byte_offset++;
     }//fin for
    }
   }
   //gb_sdl_blit=1;
    
   #ifdef use_lib_stats_time_unified
    time_prev = micros()-time_prev;
    gb_stats_video_cur_unified = time_prev;
    if (time_prev< gb_stats_video_min_unified){
     gb_stats_video_min_unified= time_prev;
    }
    if (time_prev> gb_stats_video_max_unified){
     gb_stats_video_max_unified= time_prev;
    }
   #endif
  }//fin if skipframe

  xQueueReceive(vidQueue, &param, portMAX_DELAY);
  videoTaskIsRunning = false;
  }
  videoTaskIsRunning = false;
  vTaskDelete(NULL);

  while (1)
  {
  }    

  }

  #else
  //Sin hilos 1 solo core:
  // 2272 microsegundos sin brillo activo o o 64 colores
  // 2357 con brillo activo 64 colores
  void videoTaskNoThread()
  {
 //2272 micros con array paleta
 //2561 microsegundos

 //8300 microsegundos
 //7680 microsegundos quitando desplazamiento y skip frame
 //unsigned int cont_a=0;
 unsigned char paleta[2]; //0 backcolor 1 Forecolor
 unsigned int a0,a1,a2,a3,a32;

 #ifdef use_lib_stats_time_unified
  unsigned long time_prev;
 #endif

 #ifdef use_lib_skip_frame
  unsigned char skipFrame=0;
 #endif
 int byte_offset;
 unsigned char color_attrib, flash;
 //int pixel_map;//, bright;
 int zx_vidcalc, calc_y;    

 //word zx_fore_color, zx_back_color, tmp_color;
 unsigned char zx_fore_color, zx_back_color, tmp_color;

 unsigned char auxColor;    
 unsigned char * gb_ptr_ram_1800_video;
 unsigned char * gb_ptr_ram_video;
 //unsigned char bitpos;    
 #ifdef use_lib_skip_frame
  if(
     (gbFrameSkipVideoCurrentCont < gbFrameSkipVideoMaxCont)
     && 
     (gbFrameSkipVideoMaxCont != 0)
    )
  {
   gbFrameSkipVideoCurrentCont++; 
   skipFrame = 1;
  }
  else
  { 
   gbFrameSkipVideoCurrentCont= 0;
   skipFrame = 0;
  }
 #endif


#ifdef use_lib_skip_frame
if (skipFrame == 1){
 return;
}
#endif


 if (!video_latch)
 {
  gb_ptr_ram_1800_video = &ram5[0x1800];
  gb_ptr_ram_video = ram5;
 }
 else
 {
  gb_ptr_ram_1800_video = &ram7[0x1800];
  gb_ptr_ram_video = ram7;  
 }    

 #ifdef use_lib_stats_time_unified
  time_prev = micros();
 #endif
 for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
 {     
  auxColor = gb_cache_zxcolor[borderTemp];
  a0= auxColor;
  a32= a0 | (a0<<8) | (a0<<16) | (a0<<24);
  if (vga_lin < 3 || vga_lin > 194)
  {
   #ifdef use_lib_screen_offset
    if ((gb_screen_yIni+vga_lin)<0)
    {
     continue;
    }
   #endif      

   #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)
    for (int bor = 0; bor < 74; bor++) //296 DIV 4 32 bits
   #else
    for (int bor = 8; bor < 82; bor++) //32 DIV 4   328 DIV 4 32 bits
   #endif   
   {                    
    //ptrVGA[vga_lin][bor^2] = auxColor;
    #ifdef use_lib_screen_offset
     gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
    #else
     gb_buffer_vga32[vga_lin][bor] = a32;
    #endif 
   }                
  }
  else            
  {
   #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)
    for (int bor = 0; bor < 5; bor++) //20 DIV 4 32 bits
   #else
    for (int bor = 8; bor < 13; bor++) //32 DIV 4   52 DIV 4 32 bits
   #endif   
   {                    
     //ptrVGA[vga_lin][bor^2] = auxColor;
     //ptrVGA[vga_lin][(bor+276)^2] = auxColor;     
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+gb_screen_xIni] = a32;
      gb_buffer_vga32[vga_lin+gb_screen_yIni][bor+69+gb_screen_xIni] = a32;     
     #else
      gb_buffer_vga32[vga_lin][bor] = a32;
      gb_buffer_vga32[vga_lin][bor+69] = a32;
     #endif 
   }
   byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
          
   for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
   {                    
    //calc_y = calcY(byte_offset);                    
    calc_y = gb_lookup_calcY[(byte_offset>>5)];
    color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
    //zx_vidcalc = (ff << 3); //DIV 4 32 bits
    //zx_vidcalc= (ff<<3) >> 2;  //Ya no lo necesito

    //BEGIN El color lo saco fuera 8 pixels
    flash = (color_attrib & 0B10000000) >> 7;
    #ifdef use_lib_use_bright
     if (((color_attrib & 0B01000000) >> 6) == 0x01)
     {
      zx_fore_color = gb_cache_zxcolor_bright[(color_attrib & 0B00000111)]; //con brillo
      zx_back_color = gb_cache_zxcolor_bright[((color_attrib & 0B00111000) >> 3)];//con brillo                             
     }
     else
     {
      zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //sin brillo
      zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//sin brillo
     }                    
    #else
     zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
     zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
    #endif 
    if (flash && flashing)
    {
     swap_flash(&zx_fore_color, &zx_back_color);
    }
                    
    paleta[0]= zx_back_color;
    paleta[1]= zx_fore_color;
      
    zx_vidcalc= (ff<<1); //Lo saco fuera 32 bits

    a0= paleta[((gb_ptr_ram_video[byte_offset] >>7) & 0x01)];
    a1= paleta[((gb_ptr_ram_video[byte_offset] >>6) & 0x01)];
    a2= paleta[((gb_ptr_ram_video[byte_offset] >>5) & 0x01)];
    a3= paleta[((gb_ptr_ram_video[byte_offset] >>4) & 0x01)];
                     
    a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
    #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+5+gb_screen_xIni] = a32; //20 DIV 4 = 5
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+5] = a32; //20 DIV 4 = 5
     #endif 
    #else
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+13+gb_screen_xIni] = a32; //52 DIV 4 = 13
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+13] = a32; //52 DIV 4 = 13
     #endif 
    #endif 

    a0= paleta[((gb_ptr_ram_video[byte_offset] >>3) & 0x01)];
    a1= paleta[((gb_ptr_ram_video[byte_offset] >>2) & 0x01)];
    a2= paleta[((gb_ptr_ram_video[byte_offset] >>1) & 0x01)];
    a3= paleta[((gb_ptr_ram_video[byte_offset]) & 0x01)];
                     
    a32= a2 | (a3<<8) | (a0<<16) | (a1<<24);
    #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+6+gb_screen_xIni] = a32; //20 DIV 4 = 5
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+6] = a32; //20 DIV 4 = 5
     #endif 
    #else
     #ifdef use_lib_screen_offset
      gb_buffer_vga32[(calc_y + 3+gb_screen_yIni)][zx_vidcalc+14+gb_screen_xIni] = a32; //52 DIV 4 = 13      
     #else
      gb_buffer_vga32[(calc_y + 3)][zx_vidcalc+14] = a32; //52 DIV 4 = 13      
     #endif 
    #endif 
                

    byte_offset++;
   }//fin for
  }
 }
 //gb_sdl_blit=1;


 #ifdef use_lib_stats_time_unified
  time_prev = micros()-time_prev;
  gb_stats_video_cur_unified = time_prev;
  if (time_prev< gb_stats_video_min_unified){
   gb_stats_video_min_unified= time_prev;
  }
  if (time_prev> gb_stats_video_max_unified){
   gb_stats_video_max_unified= time_prev;
  }
 #endif


 //time_prev = micros()-time_prev;
 //#ifdef use_lib_log_serial
 // Serial.printf("Tiempo %d\n",time_prev);             
 //#endif
}
#endif
#endif

//#endif



//void swap_flash(word *a, word *b) 
inline void swap_flash(unsigned char *a, unsigned char *b) 
{
 //word temp = *a;
 unsigned char temp= *a;
 *a = *b;
 *b = temp;
}





// Load zx keyboard lines from PS/2
void do_keyboard() 
{
  //byte kempston = 0; //No se usa

  #ifdef use_lib_remap_keyboardpc
   //Cursores menu en 128k
   if (keymap[KEY_CURSOR_UP] == 0){ 
    keymap[0x12]= keymap[0x3d] = 0; //shift+7
   }
   else
   {
    if (keymap[KEY_CURSOR_DOWN] == 0) { keymap[0x12]= keymap[0x36]= 0; }
    //else {keymap[0x12]= keymap[0x36]= 1;}//shift+6   
   }
  #endif 

    //if (!keymap[KEY_F1])
    //{
    // gb_show_osd_main_menu = 1;
    // do_tinyOSD();
    //}     

    bitWrite(z80ports_in[0], 0, keymap[0x12]); //SHIFT LEFT
    bitWrite(z80ports_in[0], 1, keymap[0x1a]); //Z
    bitWrite(z80ports_in[0], 2, keymap[0x22]); //X
    bitWrite(z80ports_in[0], 3, keymap[0x21]); //C
    bitWrite(z80ports_in[0], 4, keymap[0x2a]); //V

    bitWrite(z80ports_in[1], 0, keymap[0x1c]); //A
    bitWrite(z80ports_in[1], 1, keymap[0x1b]); //S
    bitWrite(z80ports_in[1], 2, keymap[0x23]); //D
    bitWrite(z80ports_in[1], 3, keymap[0x2b]); //F
    bitWrite(z80ports_in[1], 4, keymap[0x34]); //G

    bitWrite(z80ports_in[2], 0, keymap[0x15]); //Q
    bitWrite(z80ports_in[2], 1, keymap[0x1d]); //W
    bitWrite(z80ports_in[2], 2, keymap[0x24]); //E
    bitWrite(z80ports_in[2], 3, keymap[0x2d]); //R
    bitWrite(z80ports_in[2], 4, keymap[0x2c]); //T

    bitWrite(z80ports_in[3], 0, keymap[0x16]); //1
    bitWrite(z80ports_in[3], 1, keymap[0x1e]); //2
    bitWrite(z80ports_in[3], 2, keymap[0x26]); //3
    bitWrite(z80ports_in[3], 3, keymap[0x25]); //4
    bitWrite(z80ports_in[3], 4, keymap[0x2e]); //5

    bitWrite(z80ports_in[4], 0, keymap[0x45]); //0
    bitWrite(z80ports_in[4], 1, keymap[0x46]); //9
    bitWrite(z80ports_in[4], 2, keymap[0x3e]); //8
    bitWrite(z80ports_in[4], 3, keymap[0x3d]); //7
    bitWrite(z80ports_in[4], 4, keymap[0x36]); //6

    bitWrite(z80ports_in[5], 0, keymap[0x4d]); //P
    bitWrite(z80ports_in[5], 1, keymap[0x44]); //O
    bitWrite(z80ports_in[5], 2, keymap[0x43]); //I
    bitWrite(z80ports_in[5], 3, keymap[0x3c]); //U
    bitWrite(z80ports_in[5], 4, keymap[0x35]); //Y

    bitWrite(z80ports_in[6], 0, keymap[0x5a]); //ENTER
    bitWrite(z80ports_in[6], 1, keymap[0x4b]); //L
    bitWrite(z80ports_in[6], 2, keymap[0x42]); //K
    bitWrite(z80ports_in[6], 3, keymap[0x3b]); //J
    bitWrite(z80ports_in[6], 4, keymap[0x33]); //H

    bitWrite(z80ports_in[7], 0, keymap[0x29]); //Barra espaciadora
    bitWrite(z80ports_in[7], 1, keymap[0x14]); //CONTROL
    bitWrite(z80ports_in[7], 2, keymap[0x3a]); //M
    bitWrite(z80ports_in[7], 3, keymap[0x31]); //N
    bitWrite(z80ports_in[7], 4, keymap[0x32]); //B

    //Serial.printf("Teclado %d %d\n",keymap[KEY_CURSOR_UP],keymap[KEY_CURSOR_DOWN]);
    // Kempston joystick
    z80ports_in[0x1f] = 0;
    bitWrite(z80ports_in[0x1f], 0, !keymap[KEY_CURSOR_RIGHT]);
    bitWrite(z80ports_in[0x1f], 1, !keymap[KEY_CURSOR_LEFT]);
    bitWrite(z80ports_in[0x1f], 2, !keymap[KEY_CURSOR_DOWN]);
    bitWrite(z80ports_in[0x1f], 3, !keymap[KEY_CURSOR_UP]);
    bitWrite(z80ports_in[0x1f], 4, !keymap[KEY_ALT_GR]);    

 #ifdef use_lib_remap_keyboardpc
  if (keymap[KEY_CURSOR_UP] == 0){ 
   keymap[0x12]= keymap[0x3d] = 1; //shift+7
  }
  else
  {
   if (keymap[KEY_CURSOR_DOWN] == 0) { keymap[0x12]= keymap[0x36]= 1; }
   //else {keymap[0x12]= keymap[0x36]= 1;}//shift+6   
  }
 #endif 
}

#ifdef use_lib_keyboard_uart
 //const int BUFFER_SIZE = 50;
 char gb_buf_uart[BUFFER_SIZE_UART];
 unsigned char gb_rlen_uart=0;

 #ifdef use_lib_core_linkefong
 void do_keyboard_uart()
 {  
  unsigned int contBuf=0;
  //if(Serial.available() > 0)
  if (gb_rlen_uart>0)
  {
//   gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);

   //Serial.print("I received: ");
//   gb_buf_uart[gb_rlen_uart]='\0';
   //Serial.printf("Tot:%d\nASCII:%s\n",gb_rlen_uart,gb_buf_uart);  
   //Serial.printf("Buf:");   
   //Serial.printf("\n"); 
  //}
   
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x01: //F2   01 62 4F 51      
      if ((contBuf+3) < gb_rlen_uart)
      {
       gb_show_osd_main_menu= 1;
       #ifdef use_lib_sound_ay8912   
        gb_silence_all_channels = 1;
       #endif
       contBuf+= 3;
      }
      break;

     case 0x09: //TAB saco menu tambien
      gb_show_osd_main_menu= 1;
      #ifdef use_lib_sound_ay8912   
       gb_silence_all_channels = 1;
      #endif
      break;

     case 0x08: case 0x7F: //Borrar SHIFT+0
       bitWrite(z80ports_in[0], 0, 0); //SHIFT
       bitWrite(z80ports_in[4], 0, 0); //0
       //0x08 en VStudio, 0x7F en putty
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
      //if ((contBuf+1) < gb_rlen_uart)
      //{
      // contBuf++;
      // if (gb_buf_uart[contBuf] == 0x0A)
      // {
      //  bitWrite(z80ports_in[6], 0, 0); //ENTER
      //  //contBuf++;
      // }
      //}
       bitWrite(z80ports_in[6], 0, 0); //ENTER
       //Serial.printf("ENTER\r\n");
       //OD 0A en VStudio, 0D en putty
      break;

     case 0x22: //Teclado PC Comillas shift+2 (zx48K control + p)       
       bitWrite(z80ports_in[7], 1, 0); //CONTROL *
       bitWrite(z80ports_in[5], 0, 0); //P
      break;

     case 0x2B: bitWrite(z80ports_in[0x1f], 4, 1); //El + como ALT_GR
      break;
     
     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: 
          bitWrite(z80ports_in[0x1f], 3, 1); //kempston arriba
          #ifdef use_lib_remap_keyboardpc
           //shift+7
           bitWrite(z80ports_in[0], 0, 0); //SHIFT
           bitWrite(z80ports_in[4], 3, 0); //7
          #endif
          //arriba 1B 5B 41
          break;
         case 0x42: 
          bitWrite(z80ports_in[0x1f], 2, 1);  //kempston abajo
          #ifdef use_lib_remap_keyboardpc
           //shift+6
           bitWrite(z80ports_in[0], 0, 0); //SHIFT
           bitWrite(z80ports_in[4], 4, 0); //6
          #endif
          break; //abajo 1B 5B 42
         case 0x43: bitWrite(z80ports_in[0x1f], 0, 1); break; //derecha 1B 5B 43
         case 0x44: bitWrite(z80ports_in[0x1f], 1, 1); break; //izquierda 1B 5B 44        
         //z80ports_in[0x1f] = 0;
         //bitWrite(z80ports_in[0x1f], 0, !keymap[KEY_CURSOR_RIGHT]);
         //bitWrite(z80ports_in[0x1f], 1, !keymap[KEY_CURSOR_LEFT]);
         //bitWrite(z80ports_in[0x1f], 2, !keymap[KEY_CURSOR_DOWN]);
         //bitWrite(z80ports_in[0x1f], 3, !keymap[KEY_CURSOR_UP]);
         //bitWrite(z80ports_in[0x1f], 4, !keymap[KEY_ALT_GR]);
        }
       }
      }
      break; 

     //bitWrite(z80ports_in[0], 0, keymap[0x12]); //SHIFT LEFT
     case 0x2D: bitWrite(z80ports_in[0], 0, 0); break; //SHIFT LEFT -
     case 0x7A: case 0x5A: bitWrite(z80ports_in[0], 1, 0); break; //Z
     case 0x78: case 0x58: bitWrite(z80ports_in[0], 2, 0); break; //X
     case 0x63: case 0x43: bitWrite(z80ports_in[0], 3, 0); break; //C
     case 0x76: case 0x56: bitWrite(z80ports_in[0], 4, 0); break; //V

     case 0x61: case 0x41: bitWrite(z80ports_in[1], 0, 0); break; //A
     case 0x73: case 0x53: bitWrite(z80ports_in[1], 1, 0); break; //S
     case 0x64: case 0x44: bitWrite(z80ports_in[1], 2, 0); break; //D
     case 0x66: case 0x46: bitWrite(z80ports_in[1], 3, 0); break; //F
     case 0x67: case 0x47: bitWrite(z80ports_in[1], 4, 0); break; //G

     case 0x71: case 0x51: bitWrite(z80ports_in[2], 0, 0); break; //Q
     case 0x77: case 0x57: bitWrite(z80ports_in[2], 1, 0); break; //W
     case 0x65: case 0x45: bitWrite(z80ports_in[2], 2, 0); break; //E
     case 0x72: case 0x52: bitWrite(z80ports_in[2], 3, 0); break; //R
     case 0x74: case 0x54: bitWrite(z80ports_in[2], 4, 0); break; //T

     case 0x31: bitWrite(z80ports_in[3], 0, 0); break; //1
     case 0x32: bitWrite(z80ports_in[3], 1, 0); break; //2
     case 0x33: bitWrite(z80ports_in[3], 2, 0); break; //3
     case 0x34: bitWrite(z80ports_in[3], 3, 0); break; //4
     case 0x35: bitWrite(z80ports_in[3], 4, 0); break; //5

     case 0x30: bitWrite(z80ports_in[4], 0, 0); break; //0
     case 0x39: bitWrite(z80ports_in[4], 1, 0); break; //9
     case 0x38: bitWrite(z80ports_in[4], 2, 0); break; //8
     case 0x37: bitWrite(z80ports_in[4], 3, 0); break; //7
     case 0x36: bitWrite(z80ports_in[4], 4, 0); break; //6

     case 0x70: case 0x50: bitWrite(z80ports_in[5], 0, 0); break; //P
     case 0x6F: case 0x4F: bitWrite(z80ports_in[5], 1, 0); break; //O
     case 0x69: case 0x49: bitWrite(z80ports_in[5], 2, 0); break; //I
     case 0x75: case 0x55: bitWrite(z80ports_in[5], 3, 0); break; //U
     case 0x79: case 0x59: bitWrite(z80ports_in[5], 4, 0); break; //Y

     //bitWrite(z80ports_in[6], 0, 0); //ENTER
     case 0x6C: case 0x4C: bitWrite(z80ports_in[6], 1, 0); break; //L
     case 0x6B: case 0x4B: bitWrite(z80ports_in[6], 2, 0); break; //K
     case 0x6A: case 0x4A: bitWrite(z80ports_in[6], 3, 0); break; //J
     case 0x68: case 0x48: bitWrite(z80ports_in[6], 4, 0); break; //H

     case 0x20: bitWrite(z80ports_in[7], 0, 0); break; //Barra espaciadora
     //bitWrite(z80ports_in[7], 1, 0); break; //CONTROL
     case 0x2A: bitWrite(z80ports_in[7], 1, 0); break; //CONTROL *
     case 0x6D: case 0x4D: bitWrite(z80ports_in[7], 2, 0); break; //M
     case 0x6E: case 0x4E: bitWrite(z80ports_in[7], 3, 0); break; //N
     case 0x62: case 0x42: bitWrite(z80ports_in[7], 4, 0); break; //B
    }

    contBuf++;
   }
    //bitWrite(z80ports_in[0], 0, keymap[0x12]);
    //bitWrite(z80ports_in[0], 1, keymap[0x1a]); //Z
    //bitWrite(z80ports_in[0], 2, keymap[0x22]); //X
    //bitWrite(z80ports_in[0], 3, keymap[0x21]); //C
    //bitWrite(z80ports_in[0], 4, keymap[0x2a]); //V

    //bitWrite(z80ports_in[1], 0, keymap[0x1c]); //A
    //bitWrite(z80ports_in[1], 1, keymap[0x1b]); //S
    //bitWrite(z80ports_in[1], 2, keymap[0x23]); //D
    //bitWrite(z80ports_in[1], 3, keymap[0x2b]); //F
    //bitWrite(z80ports_in[1], 4, keymap[0x34]); //G    
  }
 }
 #else
  #ifdef use_lib_core_jsanchezv
 void do_keyboard_uart()
 {  
  unsigned int contBuf=0;  
  if (gb_rlen_uart>0)
  {   
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x01: //F2   01 62 4F 51      
      if ((contBuf+3) < gb_rlen_uart)
      {
       gb_show_osd_main_menu= 1;
       #ifdef use_lib_sound_ay8912   
        gb_silence_all_channels = 1;
       #endif
       contBuf+= 3;
      }
      break;

     case 0x09: //TAB saco menu tambien
      gb_show_osd_main_menu= 1;
      #ifdef use_lib_sound_ay8912   
       gb_silence_all_channels = 1;
      #endif
      break;

     case 0x08: case 0x7F: //Borrar SHIFT+0
       bitWrite(z80Ports_jsanchezv[0], 0, 0); //SHIFT
       bitWrite(z80Ports_jsanchezv[4], 0, 0); //0
       //0x08 en VStudio, 0x7F en putty
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
       bitWrite(z80Ports_jsanchezv[6], 0, 0); //ENTER
       //Serial.printf("ENTER\r\n");
       //OD 0A en VStudio, 0D en putty
      break;

     case 0x22: //Teclado PC Comillas shift+2 (zx48K control + p)       
       bitWrite(z80Ports_jsanchezv[7], 1, 0); //CONTROL *
       bitWrite(z80Ports_jsanchezv[5], 0, 0); //P
      break;      

     case 0x2B: bitWrite(z80Ports_jsanchezv[0x1f], 4, 1); //El + como ALT_GR
      break;

     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: 
          bitWrite(z80Ports_jsanchezv[0x1f], 3, 1); //kempston arriba
          #ifdef use_lib_remap_keyboardpc
           //shift+7
           bitWrite(z80Ports_jsanchezv[0], 0, 0); //SHIFT
           bitWrite(z80Ports_jsanchezv[4], 3, 0); //7
          #endif
          //arriba 1B 5B 41
          break;
         case 0x42: 
          bitWrite(z80Ports_jsanchezv[0x1f], 2, 1);  //kempston abajo
          #ifdef use_lib_remap_keyboardpc
           //shift+6
           bitWrite(z80Ports_jsanchezv[0], 0, 0); //SHIFT
           bitWrite(z80Ports_jsanchezv[4], 4, 0); //6
          #endif
          break; //abajo 1B 5B 42
         case 0x43: bitWrite(z80Ports_jsanchezv[0x1f], 0, 1); break; //derecha 1B 5B 43
         case 0x44: bitWrite(z80Ports_jsanchezv[0x1f], 1, 1); break; //izquierda 1B 5B 44        
         //z80ports_in[0x1f] = 0;
         //bitWrite(z80ports_in[0x1f], 0, !keymap[KEY_CURSOR_RIGHT]);
         //bitWrite(z80ports_in[0x1f], 1, !keymap[KEY_CURSOR_LEFT]);
         //bitWrite(z80ports_in[0x1f], 2, !keymap[KEY_CURSOR_DOWN]);
         //bitWrite(z80ports_in[0x1f], 3, !keymap[KEY_CURSOR_UP]);
         //bitWrite(z80ports_in[0x1f], 4, !keymap[KEY_ALT_GR]);
        }
       }
      }
      break; 

     //bitWrite(z80ports_in[0], 0, keymap[0x12]); //SHIFT LEFT
     case 0x2D: bitWrite(z80Ports_jsanchezv[0], 0, 0); break; //SHIFT LEFT -
     case 0x7A: case 0x5A: bitWrite(z80Ports_jsanchezv[0], 1, 0); break; //Z
     case 0x78: case 0x58: bitWrite(z80Ports_jsanchezv[0], 2, 0); break; //X
     case 0x63: case 0x43: bitWrite(z80Ports_jsanchezv[0], 3, 0); break; //C
     case 0x76: case 0x56: bitWrite(z80Ports_jsanchezv[0], 4, 0); break; //V

     case 0x61: case 0x41: bitWrite(z80Ports_jsanchezv[1], 0, 0); break; //A
     case 0x73: case 0x53: bitWrite(z80Ports_jsanchezv[1], 1, 0); break; //S
     case 0x64: case 0x44: bitWrite(z80Ports_jsanchezv[1], 2, 0); break; //D
     case 0x66: case 0x46: bitWrite(z80Ports_jsanchezv[1], 3, 0); break; //F
     case 0x67: case 0x47: bitWrite(z80Ports_jsanchezv[1], 4, 0); break; //G

     case 0x71: case 0x51: bitWrite(z80Ports_jsanchezv[2], 0, 0); break; //Q
     case 0x77: case 0x57: bitWrite(z80Ports_jsanchezv[2], 1, 0); break; //W
     case 0x65: case 0x45: bitWrite(z80Ports_jsanchezv[2], 2, 0); break; //E
     case 0x72: case 0x52: bitWrite(z80Ports_jsanchezv[2], 3, 0); break; //R
     case 0x74: case 0x54: bitWrite(z80Ports_jsanchezv[2], 4, 0); break; //T

     case 0x31: bitWrite(z80Ports_jsanchezv[3], 0, 0); break; //1
     case 0x32: bitWrite(z80Ports_jsanchezv[3], 1, 0); break; //2
     case 0x33: bitWrite(z80Ports_jsanchezv[3], 2, 0); break; //3
     case 0x34: bitWrite(z80Ports_jsanchezv[3], 3, 0); break; //4
     case 0x35: bitWrite(z80Ports_jsanchezv[3], 4, 0); break; //5

     case 0x30: bitWrite(z80Ports_jsanchezv[4], 0, 0); break; //0
     case 0x39: bitWrite(z80Ports_jsanchezv[4], 1, 0); break; //9
     case 0x38: bitWrite(z80Ports_jsanchezv[4], 2, 0); break; //8
     case 0x37: bitWrite(z80Ports_jsanchezv[4], 3, 0); break; //7
     case 0x36: bitWrite(z80Ports_jsanchezv[4], 4, 0); break; //6

     case 0x70: case 0x50: bitWrite(z80Ports_jsanchezv[5], 0, 0); break; //P
     case 0x6F: case 0x4F: bitWrite(z80Ports_jsanchezv[5], 1, 0); break; //O
     case 0x69: case 0x49: bitWrite(z80Ports_jsanchezv[5], 2, 0); break; //I
     case 0x75: case 0x55: bitWrite(z80Ports_jsanchezv[5], 3, 0); break; //U
     case 0x79: case 0x59: bitWrite(z80Ports_jsanchezv[5], 4, 0); break; //Y

     //bitWrite(z80ports_in[6], 0, 0); //ENTER
     case 0x6C: case 0x4C: bitWrite(z80Ports_jsanchezv[6], 1, 0); break; //L
     case 0x6B: case 0x4B: bitWrite(z80Ports_jsanchezv[6], 2, 0); break; //K
     case 0x6A: case 0x4A: bitWrite(z80Ports_jsanchezv[6], 3, 0); break; //J
     case 0x68: case 0x48: bitWrite(z80Ports_jsanchezv[6], 4, 0); break; //H

     case 0x20: bitWrite(z80Ports_jsanchezv[7], 0, 0); break; //Barra espaciadora
     //bitWrite(z80ports_in[7], 1, 0); break; //CONTROL
     case 0x2A: bitWrite(z80Ports_jsanchezv[7], 1, 0); break; //CONTROL *
     case 0x6D: case 0x4D: bitWrite(z80Ports_jsanchezv[7], 2, 0); break; //M
     case 0x6E: case 0x4E: bitWrite(z80Ports_jsanchezv[7], 3, 0); break; //N
     case 0x62: case 0x42: bitWrite(z80Ports_jsanchezv[7], 4, 0); break; //B
    }

    contBuf++;
   }
    //bitWrite(z80ports_in[0], 0, keymap[0x12]);
    //bitWrite(z80ports_in[0], 1, keymap[0x1a]); //Z
    //bitWrite(z80ports_in[0], 2, keymap[0x22]); //X
    //bitWrite(z80ports_in[0], 3, keymap[0x21]); //C
    //bitWrite(z80ports_in[0], 4, keymap[0x2a]); //V

    //bitWrite(z80ports_in[1], 0, keymap[0x1c]); //A
    //bitWrite(z80ports_in[1], 1, keymap[0x1b]); //S
    //bitWrite(z80ports_in[1], 2, keymap[0x23]); //D
    //bitWrite(z80ports_in[1], 3, keymap[0x2b]); //F
    //bitWrite(z80ports_in[1], 4, keymap[0x34]); //G    
  }
 }  
  #endif
 #endif
#endif 

#ifdef use_lib_remap_keyboardpc 
 //Cursores menu en 128k
 void do_keyboard_remap_pc()
 { 
  if (checkAndCleanKey(KEY_BACKSPACE) || checkAndCleanKey(KEY_DELETE))
   keymap[0x12]= keymap[0x45]=0;  //shift+0
   
  //if (checkKey(KEY_CURSOR_UP))
  // keymap[0x12]= keymap[0x3d] = 0; //shift+7    

  //if (checkKey(KEY_CURSOR_DOWN))
  // keymap[0x12]= keymap[0x36] = 0;  //shift+6
 }
#endif 


// +-------------+
// | LOOP core 1 |
// +-------------+
//
void loop() {
    // static byte last_ts = 0;
    //unsigned long ts1, ts2;
    #ifdef use_lib_core_linkefong
     if (halfsec) {
         flashing = ~flashing;
     }
     sp_int_ctr++;
     halfsec = !(sp_int_ctr % 25);
    #else
     #ifdef use_lib_core_jsanchezv
      if (halfsec_jsanchezv) 
      {
       flashing_jsanchezv = ~flashing_jsanchezv;
      }
      sp_int_ctr_jsanchezv++;
      halfsec_jsanchezv = !(sp_int_ctr_jsanchezv % 25);
     #endif 
    #endif

    gb_currentTime = millis();
    if ((gb_currentTime-gb_keyboardTime) >= gb_current_ms_poll_keyboard)
    {
     gb_keyboardTime = gb_currentTime;         
     #ifdef use_lib_core_linkefong
      #ifdef use_lib_remap_keyboardpc 
       do_keyboard_remap_pc();
      #endif
      do_keyboard();
      #ifdef use_lib_keyboard_uart       
       gb_curTime_keyboard_uart= gb_currentTime;
       if ((gb_curTime_keyboard_uart - gb_curTime_keyboard_before_uart) >= gb_current_ms_poll_keyboard_uart)
       {
        gb_curTime_keyboard_before_uart= gb_curTime_keyboard_uart;
        keyboard_uart_poll();
       }
       do_keyboard_uart();       
      #endif
      //if (gb_show_osd_main_menu == 1)
      //{
      // do_tinyOSD();
      //}      
     #else
      #ifdef use_lib_remap_keyboardpc 
       do_keyboard_remap_pc();
      #endif     
      keyboard_do_jsanchezv();
      #ifdef use_lib_keyboard_uart       
       gb_curTime_keyboard_uart= gb_currentTime;
       if ((gb_curTime_keyboard_uart - gb_curTime_keyboard_before_uart) >= gb_current_ms_poll_keyboard_uart)
       {
        gb_curTime_keyboard_before_uart= gb_curTime_keyboard_uart;
        keyboard_uart_poll();
       }
       do_keyboard_uart();       
      #endif      
      //if (gb_show_osd_main_menu == 1)
      //{
      // do_tinyOSD();
      //}
     #endif 
    }    
    do_tinyOSD();

    #ifdef use_lib_mouse_kempston
     gb_currentTime = millis();
     if ((gb_currentTime-gb_mouseTime) >= gb_current_ms_poll_mouse)
     {
      gb_mouseTime = gb_currentTime;
      if (gb_mouse_init_error == 0)
      {    
       PollMouse(); //Debe procesar despues teclado y antes de EMU
      }
     }
    #endif     
            
    if ((gb_current_delay_emulate_ms == 0) || (gb_run_emulacion == 1))
    {//Ejecutamos emulacion z80    
     // ts1 = millis();         
     #ifdef use_lib_core_linkefong              
      #ifdef use_lib_stats_time_unified
       gb_ini_cpu_micros_unified= micros();
       gb_fps_unified++;       
       zx_loop();
       gb_fin_cpu_micros_unified= micros();       

       gb_stats_time_cur_unified= (gb_fin_cpu_micros_unified-gb_ini_cpu_micros_unified);
       if (gb_stats_time_cur_unified < gb_stats_time_min_unified){
        gb_stats_time_min_unified=gb_stats_time_cur_unified;
       }
       if (gb_stats_time_cur_unified > gb_stats_time_max_unified){
        gb_stats_time_max_unified = gb_stats_time_cur_unified;
       }
      #else
       zx_loop();
      #endif
     #else
      sim.runTestJJ_poll();
     #endif      
     // ts2 = millis();     
    }    

    #ifdef use_lib_core_linkefong        
     if (gb_current_delay_emulate_ms != 0)
     {
      if (gb_sdl_blit == 1)
      {
       gb_sdl_blit= 0;
       gb_run_emulacion= 0;
       gb_time_ini_espera = millis();
      }
     }    
    #else
     //loop_jsanchezv();      
     //sim.runTestJJ_poll();
     if (gb_current_delay_emulate_ms != 0)
     {
      if (gb_sdl_blit == 1)
      {
       gb_sdl_blit= 0;
       gb_run_emulacion= 0;
       gb_time_ini_espera = millis();
      }
     }     
    #endif 
    
   
    #ifdef use_lib_sound_ay8912
     if (gb_silence_all_channels == 1)
      SilenceAllChannels();
     else 
     {
      gb_currentTime = millis();
      if ((gb_currentTime-gb_sdl_time_sound_before) >= gb_current_ms_poll_sound)
      {             
       gb_sdl_time_sound_before= gb_currentTime;
       jj_mixpsg();
       sound_cycleFabgl();
      }
     }
    #endif 


    #ifdef use_lib_core_linkefong
     #ifdef use_lib_vga_thread
      xQueueSend(vidQueue, &param, portMAX_DELAY);
      while (videoTaskIsRunning)
      {
      }
      gb_sdl_blit = 1;
     #else
      gbTimeVideoNow = millis();
      if ((gbTimeVideoNow - gbTimeVideoIni) >= (gbDelayVideo-1))
      {
       gbTimeVideoIni = gbTimeVideoNow;
       videoTaskNoThread();
       gb_sdl_blit= 1;      
      }
     #endif
    #endif 

    #ifdef use_lib_core_linkefong
     gb_currentTime = millis();     
     if (gb_run_emulacion == 0)    
      if ((gb_currentTime - gb_time_ini_espera) >= (gb_current_delay_emulate_ms-1))
       gb_run_emulacion = 1;
    #endif


    #ifdef use_lib_core_jsanchezv
     //if ((gb_currentTime - gbSDLVideoTimeBefore) >= 50)
     //if (vsync_jsanchezv == 1)
     //{      
      //gbSDLVideoTimeBefore= gb_currentTime;      
     // vsync_jsanchezv= 0;
      //if ((gb_fps_jsanchezv & 0x01)==0x01)
     // {
//       videoTaskNoThread_jsanchezv();
     // }
     //}
     #ifdef use_lib_vga_thread
      xQueueSend(vidQueue, &param, portMAX_DELAY);
      while (videoTaskIsRunning)
      {
      }
      gb_sdl_blit = 1;      
     #else
      gbTimeVideoNow = millis();
      if ((gbTimeVideoNow - gbTimeVideoIni) >= (gbDelayVideo-1))
      {
       gbTimeVideoIni = gbTimeVideoNow;
       videoTaskNoThread_jsanchezv();       
      }
      gb_sdl_blit = 1;
     #endif
    #endif

    #ifdef use_lib_core_jsanchezv
     gb_currentTime = millis();     
     if (gb_run_emulacion == 0)    
      if ((gb_currentTime - gb_time_ini_espera) >= (gb_current_delay_emulate_ms-1))
       gb_run_emulacion = 1;
    #endif

    
    //Medicion tiempos Lin Ke-Fong y JLS
    #ifdef use_lib_stats_time_unified
     gb_currentTime= millis();
     if ((gb_currentTime - gb_fps_time_ini_unified) > 1000)
     {
      gb_fps_time_ini_unified= gb_currentTime;
      unsigned int aux_fps= gb_fps_unified - gb_fps_ini_unified;
      gb_fps_ini_unified = gb_fps_unified;
      Serial.printf ("fps:%d %d m:%d mx:%d v %d m:%d mx:%d\r\n",aux_fps,gb_stats_time_cur_unified,gb_stats_time_min_unified,gb_stats_time_max_unified,gb_stats_video_cur_unified,gb_stats_video_min_unified,gb_stats_video_max_unified);
      gb_stats_time_min_unified = 500000;
      gb_stats_time_max_unified = 0;
      gb_stats_video_min_unified = 500000;
      gb_stats_video_max_unified = 0;
     }
    #endif




    //#ifdef use_lib_core_jsanchezv       
    // #ifdef use_lib_stats_time_jsanchezv
    //  if ((gb_currentTime - gb_fps_time_ini_jsanchezv )>1000)
    //  {//Medir fps core jose luis
    //   gb_fps_time_ini_jsanchezv= gb_currentTime;
    //   unsigned long aux_fps= gb_fps_jsanchezv-gb_fps_ini_jsanchezv;
    //   gb_fps_ini_jsanchezv = gb_fps_jsanchezv;
    //   #ifdef use_lib_stats_video_jsanchezv       
    //    Serial.printf ("fps:%d %d m:%d mx:%d v %d m:%d mx:%d\n",aux_fps,gb_stats_time_cur_jsanchezv,gb_stats_time_min_jsanchezv,gb_stats_time_max_jsanchezv,gb_stats_video_cur_jsanchezv,gb_stats_video_min_jsanchezv,gb_stats_video_max_jsanchezv);
    //   #else
    //    Serial.printf ("fps:%d %d m:%d mx:%d\n",aux_fps,gb_stats_time_cur_jsanchezv,gb_stats_time_min_jsanchezv,gb_stats_time_max_jsanchezv);
    //   #endif 
    //   //gb_stats_time_cur_jsanchezv=0;//Reseteamos estadisticas       
    //   gb_stats_time_min_jsanchezv=500000;
    //   gb_stats_time_max_jsanchezv=0;
    //   #ifdef use_lib_stats_video_jsanchezv
    //    gb_stats_video_min_jsanchezv=500000;
    //    gb_stats_video_max_jsanchezv=0;
    //   #endif 
	//  }
    // #endif 
    //#endif

//videoTaskNoThread(); //Video sin hilos

    
    //if ((ts2 - ts1) != last_ts) {
    //    Serial.printf("PC:  %d time: %d\n", _zxCpu.pc, ts2 - ts1);
    //    last_ts = ts2 - ts1;
    //}


   #ifdef use_lib_vga_thread
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
    vTaskDelay(0); // important to avoid task watchdog timeouts - change this to slow down emu
   #endif    
}
