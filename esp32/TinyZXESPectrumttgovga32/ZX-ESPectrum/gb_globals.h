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
#endif
