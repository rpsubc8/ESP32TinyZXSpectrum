#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 //SELECT use_optimice_writebyte OR use_optimice_writebyte_min_sram
 //use_optimice_writebyte required 16KB SRAM
 //use_optimice_writebyte_min_sram not required 16KB SRAM
 //#define use_optimice_writebyte
 //#define use_optimice_writebyte_min_sram

 #ifdef use_optimice_writebyte_min_sram
  #ifndef use_optimice_writebyte
   #define use_optimice_writebyte
  #endif 
 #endif

 //Not use double numbers (IEEE not fpu) calculate vga frequency
 //#define use_lib_fix_double_precision

 //Debug i2s bitluni vga frequency
 //#define use_lib_debug_i2s

 //Enable WIFI SNA, SCR. 128K is not allowed, only 64K.
 //#define use_lib_wifi
 #ifdef use_lib_wifi
  #define use_lib_only_48k
 #endif
 //WEMOS D1 R32 board is currently not supported.
 //#define use_lib_board_wemos

 //Enable load screen capture from putty
 //#define use_lib_scr_uart

 //Enable load SNA from putty
 //#define use_lib_sna_uart

 //Use terminal keyboard, putty
 //#define use_lib_keyboard_uart
 
 //timeout read millis
 #define use_lib_keyboard_uart_timeout 0

 //log keyboard remote uart 
 //#define use_lib_log_keyboard_uart

 //Fixes error keyboards not initializing (solution dcrespo3D)
 //#define FIX_PERIBOARD_NOT_INITING
 
 //Using modified bitluni library in C tiny fast
 #define use_lib_tinybitluni_fast 

 //Use IRAM ram5,ram7 array for video fast only core Lin Ke-Fong
 #define use_lib_iram_video

 //Emulates mouse connected to PS/2 
 //#define use_lib_mouse_kempston
 //Wait to initialise mouse
 #define gb_delay_init_ms 1000 
 //Left-handed
 //#define use_lib_mouse_kempston_lefthanded
 //Emulates AY8912 via loudspeaker requires reduced fabgl 
 //#define use_lib_sound_ay8912
 //Experimental to re-deploy internal loudspeaker requires reduced fabgl
 //#define use_lib_resample_speaker
 //Sound recording tape speakers
 //#define use_lib_redirect_tapes_speaker
 //REGWRITE vs digitalwrite port 25
 //#define use_lib_ultrafast_speaker
 //Pre-calculated table in RAM or Flash
 #define use_lib_lookup_ram
 //Intercept routine 0x056B core Lin Ke-Fong only
 //#define use_lib_tape_rom_intercept

 //Strength 8-colour or 64-colour mode
 //#define use_lib_vga8colors
 #define use_lib_vga64colors

 //Only used in 64-colour mode.brightness mode (64 colours active)
 //#define use_lib_use_bright 

 //Experimental low RAM video mode
 //No longer use use_lib_vga_low_memory
 //#define use_lib_vga_low_memory
 //Video with threads
 #define use_lib_vga_thread
 //CPU speed optimisation video
 //Use_lib_ultrafast_vga no longer used
 //#define use_lib_ultrafast_vga
 
 //Use keyboard cursors in 128K
 #define use_lib_remap_keyboardpc
 //Being able to adjust X Y display
 //#define use_lib_screen_offset
 //Being able to skip video frames
 //#define use_lib_skip_frame
 

 //Video mode selection
 //Select one mode and deselect other modes
 //360x200 720x400 31.3 Khz 70.3 Hz freq:28322000 pixel_clock:14161000
 //#define use_lib_vga360x200x70hz_bitluni
 //320x240 640x480 31.4 Khz 60 Hz freq:25175000 pixel_clock:12587500
 //#define use_lib_vga320x240x60hz_bitluni
 //320x200 720x400 31.4 Khz 70.0 Hz freq:25175000 pixel_clock:12587500
 #define use_lib_vga320x200x70hz_bitluni
 //320x200 70Hz freq:12587500 funciona
 //#define use_lib_vga320x200x70hz_fabgl
 //QVGA 320x240 60Hz freq:12600000 funciona
 //#define use_lib_vga320x240x60hz_fabgl


 #ifdef use_lib_vga360x200x70hz_bitluni
  #define use_lib_vga360x200  
 #else
  #ifdef use_lib_vga320x240x60hz_bitluni
   #define use_lib_vga320x240   
  #else
   #ifdef use_lib_vga320x200x70hz_bitluni
    #define use_lib_vga320x200
   #else
    #ifdef use_lib_vga320x200x70hz_fabgl
     #define use_lib_vga320x200
     #ifndef use_lib_fix_double_precision
      #define use_lib_fix_double_precision
     #endif
    #else
     #ifdef use_lib_vga320x240x60hz_fabgl
      #define use_lib_vga320x240
      #ifndef use_lib_fix_double_precision
       #define use_lib_fix_double_precision
      #endif      
     #endif
    #endif
   #endif
  #endif
 #endif

 //Ya no se usa
 //#define use_lib_vga360x200
 //Ya no se usa
 //#define use_lib_vga320x200
 //Ya no se usa
 //#define use_lib_vga320x240

 //Log output usb serial port
 #define use_lib_log_serial 

 //Time measurement
 #define use_lib_stats_time_unified

 //Microseconds wait per CPU instruction (AUTO 1 adapts to frame 20000 micros)
 #define use_lib_delay_tick_cpu_auto 1
 #define use_lib_delay_tick_cpu_micros 0
 //0x01 1 instruction    0x03 4 instructions     0x07 8 instructions
 //#define use_lib_cpu_adjust_mask_instructions 0x01
 #define use_lib_cpu_adjust_mask_instructions 0x03
 //#define use_lib_cpu_adjust_mask_instructions 0x07

 //Select core Lin Ke-Fong or jsanchezv 
 #define use_lib_core_linkefong
 //core José Luis Sanchez zx81
 //#define use_lib_core_jsanchezv
 //Only used in JLS core. CPU consuming
 #define use_lib_delayContention
 //#define use_lib_stats_time_jsanchezv
 //Use_lib_stats_video_jsanchezv is no longer used.
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


 //Poll keyboard remote uart
 #define gb_current_ms_poll_keyboard_uart 50
 //Read mouse poll millis
 #define gb_ms_mouse 10
 //Read keyboard every x milliseconds
 #define gb_ms_keyboard 10
 //Read sound every x milliseconds
 #define gb_ms_sound 1
 //Skip x frames
 #define gb_frame_crt_skip 0
 //Milliseconds wait on each emulation iteration
 #define gb_delay_emulate_ms 0
 
 //Microseconds division standby emulation is no longer used.
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

 //fabgl dependence is not used
 // - use_lib_mouse_kempston
 // - use_lib_sound_ay8912
 // - use_lib_resample_speaker

 //use_lib_vga8colors - 3 pin vga
 //use_lib_use_bright - 6 pin vga
#endif
