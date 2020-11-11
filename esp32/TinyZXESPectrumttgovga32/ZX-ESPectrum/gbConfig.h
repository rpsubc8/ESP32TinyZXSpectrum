#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 //#define use_lib_mouse_kempston
 //#define use_lib_sound_ay8912
 //#define use_lib_resample_speaker
 #define use_lib_redirect_tapes_speaker
 #define use_lib_vga8colors
 //#define use_lib_use_bright 
 //#define use_lib_vga_low_memory
 #define use_lib_vga_thread
 #define use_lib_remap_keyboardpc
 #define use_lib_screen_offset
 #define use_lib_skip_frame
 //#define use_lib_vga320x200
 //#define use_lib_log_serial


 
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
