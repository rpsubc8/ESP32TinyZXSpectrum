#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H
 #include "gbConfig.h"

 //Punteros globales roms
 extern const char ** gb_ptr_list_roms_48k_title;
 extern const unsigned char ** gb_ptr_list_roms_48k_data;
 extern const char ** gb_ptr_list_roms_128k_title;
 typedef const unsigned char * array4rom[4];
 //extern const unsigned char ** gb_ptr_list_roms_128k_data;
 extern const array4rom * gb_ptr_list_roms_128k_data;
 extern unsigned char gb_max_list_rom_48; //Maximo 256
 extern unsigned char gb_max_list_rom_128; //Maximo 256
 
 //Punteros globales SNA
 extern const char ** gb_ptr_list_sna_48k_title;
 extern const unsigned char ** gb_ptr_list_sna_48k_data;
 extern const char ** gb_ptr_list_sna_128k_title;  
 extern const unsigned char ** gb_ptr_list_sna_128k_data;
 extern unsigned char gb_max_list_sna_48; //Maximo 256
 extern unsigned char gb_max_list_sna_128; //Maximo 256

 //Punteros globales cintas
 extern const char ** gb_ptr_list_tapes_48k_title;
 extern const int * gb_ptr_list_tapes_48k_size;
 extern const unsigned char ** gb_ptr_list_tapes_48k_data;
 extern const char ** gb_ptr_list_tapes_128k_title;
 extern const unsigned char ** gb_ptr_list_tapes_128k_data;
 extern unsigned char gb_max_list_tape_48; //Maximo 256
 extern unsigned char gb_max_list_tape_128; //Maximo 256

 //Punteros globales scr
 extern const char ** gb_ptr_list_scr_48k_title;
 extern const unsigned char ** gb_ptr_list_scr_48k_data;
 extern unsigned char gb_max_list_scr_48; //Maximo 256


 #ifdef use_lib_keyboard_uart
  #define BUFFER_SIZE_UART 50
  extern char gb_buf_uart[BUFFER_SIZE_UART];
  extern unsigned char gb_rlen_uart;
  extern unsigned int gb_curTime_keyboard_before_uart;
  extern unsigned int gb_curTime_keyboard_uart;
 #endif

 #if (defined use_lib_scr_uart) || (defined use_lib_sna_uart)
  #define max_buffer_uart 1024
  extern unsigned char gb_buffer_uart[max_buffer_uart]; //1KB buffer
  extern unsigned char gb_buffer_uart_dest[max_buffer_uart]; //1KB buffer
 #endif 


 #ifdef use_lib_wifi
  extern unsigned char gb_buffer_wifi[1024]; //128 * 8
  extern int gb_size_file_wifi;  
 #endif 

 extern int gb_screen_xIni;
 extern int gb_screen_yIni;

 #ifdef use_lib_mouse_kempston
  extern int gb_z80_mouse_x;
  extern int gb_z80_mouse_y;
  extern int gb_z80_mouseBtn;
  extern unsigned char gb_mouse_ps2clk;
  extern unsigned char gb_mouse_ps2data;
  extern unsigned char gb_mouse_init_error;
  extern unsigned char gb_force_left_handed;
  extern unsigned char gb_mouse_key_btn_left;
  extern unsigned char gb_mouse_key_btn_right;
  extern unsigned char gb_mouse_key_btn_middle;
  extern unsigned char gb_mouse_key_left;
  extern unsigned char gb_mouse_key_right;
  extern unsigned char gb_mouse_key_up;
  extern unsigned char gb_mouse_key_down;
  extern unsigned char gb_mouse_invert_deltax;
  extern unsigned char gb_mouse_invert_deltay;
  extern unsigned char gb_mouse_inc;
 #endif 

 #ifdef use_lib_vga_thread
  extern volatile unsigned char gb_draw_thread;
  extern volatile unsigned char gbFrameSkipVideoMaxCont;
 #else
  extern unsigned char gbFrameSkipVideoMaxCont;  
 #endif

#ifdef use_lib_sound_ay8912
 extern int gb_ay8912_reg_select;
 extern int gb_ay8912_reg_value;
 extern int gb_ay8912_A_frec_fine;
 extern int gb_ay8912_B_frec_fine;
 extern int gb_ay8912_C_frec_fine;
 extern int gb_ay8912_A_frec_course;
 extern int gb_ay8912_B_frec_course;
 extern int gb_ay8912_C_frec_course;
 extern int gb_ay8912_noise_pitch;
 extern int gb_ay8912_A_frec;
 extern int gb_ay8912_B_frec;
 extern int gb_ay8912_C_frec;
 extern int gb_ay8912_A_vol;
 extern int gb_ay8912_B_vol;
 extern int gb_ay8912_C_vol;
 extern int gb_ay8912_mixer;
 extern unsigned char gb_silence_all_channels;
 extern unsigned char gbShiftLeftVolumen;
 extern unsigned char gb_mute_sound;
 #define gb_frecuencia_ini 220
#endif 


 extern unsigned char gb_cfg_arch_is48K;

 extern unsigned char gb_auto_delay_cpu;
 extern unsigned char gb_delay_tick_cpu_micros;

 extern unsigned char z80ports_in[128];

 extern unsigned char gbDelayVideo;

 extern unsigned char gb_current_tape;
 extern int gb_contTape;
 extern int gb_tape_read;
 extern int gb_local_arrayTape[32]; //32 bloques de cinta maximo
 extern int gb_local_numBlocks; //Numero bloques cinta actual

 extern unsigned char ** gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32;
 extern unsigned char gb_sync_bits;

 extern volatile unsigned char borderTemp;
 extern volatile unsigned char keymap[256];
 extern volatile unsigned char oldKeymap[256];

 extern unsigned char gb_run_emulacion; //Ejecuta la emulacion 
 extern unsigned char gb_current_ms_poll_sound; //milisegundos muestreo
 extern unsigned char gb_current_ms_poll_keyboard; //milisegundos muestreo teclado
 extern unsigned char gb_current_ms_poll_mouse; //milisegundos muestreo raton
 //extern unsigned char gb_current_frame_crt_skip; //el actual salto de frame
 extern unsigned char gb_current_delay_emulate_ms; //la espera en cada iteracion 
 //extern unsigned char gb_current_delay_emulate_div_microsec; //Division espera microsegundos emulacion

 extern unsigned char gb_ptr_IdRomRam[4]; //Indices para readbyte y writebyte


 extern unsigned char gb_show_osd_main_menu;


  #ifdef use_lib_core_jsanchezv
  #include "jsanchezv_z80sim.h"
  //extern unsigned char * rom0_jsanchezv;
  //extern unsigned char * rom1_jsanchezv;
  //extern unsigned char * rom2_jsanchezv;
  //extern unsigned char * rom3_jsanchezv;
      
  extern unsigned char * ram1_jsanchezv;
  #ifdef use_lib_48k_iram_jsanchezv
   //extern unsigned char ram0_jsanchezv[0x4000];
   //extern unsigned char ram2_jsanchezv[0x4000];
   //extern unsigned char ram5_jsanchezv[0x4000];
  #else
   extern unsigned char * ram0_jsanchezv;
   extern unsigned char * ram2_jsanchezv;
   extern unsigned char * ram5_jsanchezv;
  #endif
  extern unsigned char * ram3_jsanchezv;
  extern unsigned char * ram4_jsanchezv;
  
  extern unsigned char * ram6_jsanchezv;
  extern unsigned char * ram7_jsanchezv;
  extern unsigned char bank_latch_jsanchezv;
  extern unsigned char * z80Ports_jsanchezv;
  extern volatile unsigned char flashing_jsanchezv;
  extern int halfsec_jsanchezv;
  extern int sp_int_ctr_jsanchezv;
  extern unsigned char zx_data_jsanchezv;
  extern unsigned char borderTemp_jsanchezv;  
  
  extern unsigned char bank_latch_jsanchezv;
  extern unsigned char video_latch_jsanchezv;
  extern unsigned char rom_latch_jsanchezv;
  extern unsigned char paging_lock_jsanchezv;
  extern unsigned char rom_in_use_jsanchezv;
  extern unsigned char sp3_rom_jsanchezv;
  extern unsigned char sp3_mode_jsanchezv;  
  
  //extern unsigned char gb_quiero_leer_sna_jsanchezv; 
  //extern unsigned char gb_id_leer_sna_jsanchezv;
  //extern unsigned char *rom0_write_fast_jsanchezv;
  extern uint8_t * gb_local_cache_ram_jsanchezv[8];  
  extern unsigned char gb_ptr_IdRomRam_jsanchezv[4];
  extern uint8_t * gb_local_cache_rom_jsanchezv[4];
  extern Z80sim * gb_ptrSim;
  #ifdef use_lib_cycle_32bits_jsanchezv
   extern unsigned int * gb_addr_states_jsanchezv;
  #else
   extern uint64_t * gb_addr_states_jsanchezv;
  #endif 

  //extern int gb_z80_mouse_x;
  //extern int gb_z80_mouse_y;
  //extern int gb_z80_mouseBtn;
  
  //extern unsigned int gb_time_state_jsanchez_ini;
  //extern unsigned int gb_fps_jsanchezv;
  //extern unsigned int gb_fps_time_ini_jsanchezv;
  //extern unsigned int gb_fps_ini_jsanchezv;
  
  extern unsigned char interruptPend_jsanchezv;
  
  extern unsigned char gbSoundSpeaker;
  //No se necesita esta en Arduino.h
  //#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
  //#define bitSet(value, bit) ((value) |= (1UL << (bit)))
  //#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
  //#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))    
 #endif 

 //49179 bytes el SNA del 48K
 #define SIZE_SNA_48K 49179
 //131103 bytes el SNA del 128K
 #define SIZE_SNA_128K 131103

#endif
