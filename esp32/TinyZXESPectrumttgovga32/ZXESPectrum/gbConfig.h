#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 //#define use_lib_board_wemos

 //Corrige teclados que no se inicializan(solucion dcrespo3D)
 //#define FIX_PERIBOARD_NOT_INITING
 
 //Usa la libreria bitluni modificada en C tiny fast
 #define use_lib_tinybitluni_fast 

 //Usar array IRAM ram5,ram7 para video Rapido Solo core Lin Ke-Fong
 #define use_lib_iram_video

 //Emula raton conectado a PS/2 requiere fabgl
 //#define use_lib_mouse_kempston
 //Espera para inicializar raton
 #define gb_delay_init_ms 1000 
 //Zurdos
 //#define use_lib_mouse_kempston_lefthanded
 //Emula AY8912 por altavoz requiere fabgl
 //#define use_lib_sound_ay8912
 //Experimental para resamplear altavoz interno requiere fabgl
 //#define use_lib_resample_speaker
 //Sonido grabar cinta altavoces
 //#define use_lib_redirect_tapes_speaker
 //REGWRITE vs digitalwrite port 25
 //#define use_lib_ultrafast_speaker
 //Tabla precalculada en RAM o Flash
 #define use_lib_lookup_ram
 //Interceptar rutina 0x056B solo core Lin Ke-Fong
 //#define use_lib_tape_rom_intercept

 //Fuerza modo 8 colores o 64 colores
 //#define use_lib_vga8colors
 #define use_lib_vga64colors

 //Solo se usa en modo 64 colores.Modo brillo (activa 64 colores)
 //#define use_lib_use_bright 

 //Modo video bajo consumo RAM experimental
 //Ya no se usa use_lib_vga_low_memory
 //#define use_lib_vga_low_memory
 //Video con hilos
 #define use_lib_vga_thread
 //Video CPU optimizacion velocidad
 //Ya no se usa use_lib_ultrafast_vga
 //#define use_lib_ultrafast_vga
 
 //Usar cursores en 128K
 #define use_lib_remap_keyboardpc
 //Poder ajustar X Y pantalla
 //#define use_lib_screen_offset
 //Poder saltarse frames de video
 //#define use_lib_skip_frame
 
 //Seleccion modo de video
 //#define use_lib_vga360x200
 #define use_lib_vga320x200
 //#define use_lib_vga320x240

 //Salida log puerto serie usb
 //#define use_lib_log_serial 

 //Medicion de tiempos
 #define use_lib_stats_time_unified

 //Microsegundos espera por instruccion CPU (AUTO 1 adapta a frame 20000 micros)
 #define use_lib_delay_tick_cpu_auto 1
 #define use_lib_delay_tick_cpu_micros 0
 //0x01 1 instruccion    0x03 4 instrucciones     0x07 8 instrucciones
 //#define use_lib_cpu_adjust_mask_instructions 0x01
 #define use_lib_cpu_adjust_mask_instructions 0x03
 //#define use_lib_cpu_adjust_mask_instructions 0x07

 //Select core Lin Ke-Fong or jsanchezv 
 #define use_lib_core_linkefong
 //core José Luis Sanchez zx81
 //#define use_lib_core_jsanchezv
 //Solo se usa en core JLS. Consume CPU
 #define use_lib_delayContention
 //#define use_lib_stats_time_jsanchezv
 //Ya no se usa use_lib_stats_video_jsanchezv
// //#define use_lib_stats_video_jsanchezv
// //#define use_lib_rom0_inRAM_jsanchezv
// //#define use_lib_48k_iram_jsanchezv
 #define use_lib_poke8_optimice_jsanchezv
 #define use_lib_poke16_optimice_jsanchezv
 #define use_lib_peek8_optimice_jsanchezv
 #define use_lib_peek16_optimice_jsanchezv
 #define use_lib_fetchOpcode_optimice_jsanchezv
 #define use_lib_inport_optimice_jsanchezv
 #define use_lib_outport_optimice_jsanchezv
 #define use_lib_interruptHandlingTime_optimice_jsanchezv
 #define use_lib_addressOnBus_optimice_jsanchezv
 #define use_lib_isActiveINT_optimice_jsanchezv
 #define use_lib_cycle_32bits_jsanchezv

 //#define WITH_BREAKPOINT_SUPPORT
 //#define WITH_EXEC_DONE


 //Read mouse poll millis
 #define gb_ms_mouse 10
 //Leer teclado cada x milisegundos
 #define gb_ms_keyboard 10
 //Leer sonido cada x milisegundos
 #define gb_ms_sound 1
 //Salta x frames
 #define gb_frame_crt_skip 0
 //Milisegundos espera en cada iteracion de emulacion
 #define gb_delay_emulate_ms 0
 
 //Ya no se usa Microsegundos division espera emulacion
 //#define gb_delay_emulate_div_microsec 5

 
 #ifdef use_lib_resample_speaker
  #define use_lib_sound_ay8912
 #endif

 #ifdef use_lib_vga64colors  
  #ifdef use_lib_use_bright
  #else
   #define use_lib_use_bright
  #endif 
 #endif

 #ifdef use_lib_use_bright
  #define COLOR_6B
 #else
  #ifdef use_lib_vga8colors
   #define COLOR_3B
  #endif
 #endif



 #ifdef use_lib_vga320x200
  #define gb_topeY 200
  #define gb_topeX_div4 80
 #else
  #ifdef use_lib_vga320x240
   #define gb_topeY 240
   #define gb_topeX_div4 80
  #else
   #ifdef use_lib_vga360x200
    #define gb_topeY 200
    #define gb_topeX_div4 90
   #endif 
  #endif
 #endif 

 //No se usa fabgl dependence
 // - use_lib_mouse_kempston
 // - use_lib_sound_ay8912
 // - use_lib_resample_speaker

 //use_lib_vga8colors - 3 pin vga
 //use_lib_use_bright - 6 pin vga
#endif
