#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 //Emula raton conectado a PS/2 requiere fabgl
 //#define use_lib_mouse_kempston
 //Emula AY8912 por altavoz requiere fabgl
 //#define use_lib_sound_ay8912
 //Experimental para resamplear altavoz interno requiere fabgl
 //#define use_lib_resample_speaker
 //Sonido grabar cinta altavoces
 #define use_lib_redirect_tapes_speaker
 //REGWRITE vs digitalwrite port 25
 //#define use_lib_ultrafast_speaker
 //Tabla precalculada en RAM o Flash
 #define use_lib_lookup_ram
 //Interceptar rutina 0x056B
 #define use_lib_tape_rom_intercept

 //Fuerza modo 8 colores o 64 colores
 //#define use_lib_vga8colors
 #define use_lib_vga64colors

 //Modo brillo (activa 64 colores)
 #define use_lib_use_bright 

 //Modo video bajo consumo RAM experimental
 //#define use_lib_vga_low_memory
 //Video con hilos
 #define use_lib_vga_thread
 //Video CPU optimizacion velocidad
 //#define use_lib_ultrafast_vga
 
 //Usar cursores en 128K
 #define use_lib_remap_keyboardpc
 //Poder ajustar X Y pantalla
 #define use_lib_screen_offset
 //Poder saltarse frames de video
 #define use_lib_skip_frame
 
 //Seleccion modo de video
 #define use_lib_vga360x200
 //#define use_lib_vga320x200
 //#define use_lib_vga320x240

 //Salida log puerto serie usb
 //#define use_lib_log_serial 

 //Select core rampa069 or jsanchezv
 #define use_lib_core_rampa069
 //core José Luis Sanchez zx81
 //#define use_lib_core_jsanchezv
 //#define WITH_BREAKPOINT_SUPPORT
 //#define WITH_EXEC_DONE


 //Leer teclado cada x milisegundos
 #define gb_ms_keyboard 10
 //Leer sonido cada x milisegundos
 #define gb_ms_sound 1
 //Salta x frames
 #define gb_frame_crt_skip 0
 //Milisegundos espera en cada iteracion de emulacion
 #define gb_delay_emulate_ms 0
  //Microsegundos division espera emulacion
 #define gb_delay_emulate_div_microsec 5

 
 #ifdef use_lib_resample_speaker
  #define use_lib_sound_ay8912
 #endif

 #ifdef use_lib_use_bright
  #define COLOR_6B
 #else
  #ifdef use_lib_vga8colors
   #define COLOR_3B
  #endif
 #endif




 //fabgl dependence
 // - use_lib_mouse_kempston
 // - use_lib_sound_ay8912
 // - use_lib_resample_speaker

 //use_lib_vga8colors - 3 pin vga
 //use_lib_use_bright - 6 pin vga
#endif
