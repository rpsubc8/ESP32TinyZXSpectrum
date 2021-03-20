#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H
 #include "gbConfig.h"
 
 extern unsigned char gb_run_emulacion; //Ejecuta la emulacion 
 extern unsigned char gb_current_ms_poll_sound; //milisegundos muestreo
 extern unsigned char gb_current_ms_poll_keyboard; //milisegundos muestreo teclado
 //extern unsigned char gb_current_frame_crt_skip; //el actual salto de frame
 extern unsigned char gb_current_delay_emulate_ms; //la espera en cada iteracion 
 extern unsigned char gb_current_delay_emulate_div_microsec; //Division espera microsegundos emulacion

 extern unsigned char gb_ptr_IdRomRam[4]; //Indices para readbyte y writebyte



  #ifdef use_lib_core_jsanchezv
  #include "jsanchezv_z80sim.h"
  //extern unsigned char * rom0_jsanchezv;
  //extern unsigned char * rom1_jsanchezv;
  //extern unsigned char * rom2_jsanchezv;
  //extern unsigned char * rom3_jsanchezv;
    
  extern unsigned char * ram0_jsanchezv;
  extern unsigned char * ram1_jsanchezv;
  extern unsigned char * ram2_jsanchezv;
  extern unsigned char * ram3_jsanchezv;
  extern unsigned char * ram4_jsanchezv;
  extern unsigned char * ram5_jsanchezv;
  extern unsigned char * ram6_jsanchezv;
  extern unsigned char * ram7_jsanchezv;
  extern unsigned char bank_latch_jsanchezv;
  extern unsigned char * z80Ports_jsanchezv;
  extern unsigned char flashing_jsanchezv;
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
 #endif 
#endif
