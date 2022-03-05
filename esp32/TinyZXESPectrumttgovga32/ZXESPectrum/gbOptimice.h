#ifndef GB_OPTIMICE_H
 #define GB_OPTIMICE_H

 #include "hardware.h"
 //extern char gb_cache_zxcolor[8];

 #define id_cache_BLACK 0
 #define id_cache_BLUE 1
 #define id_cache_CYAN 5
 #define id_cache_WHITE 7

 extern unsigned char gb_cache_zxcolor[8];
 #ifdef use_lib_use_bright
  extern unsigned char gb_cache_zxcolor_bright[8];
 #endif 

 //static unsigned char gb_cache_zxcolor[8]=
 //{//cache de colores optimizado
 // BLACK, BLUE, RED, MAGENTA, GREEN, CYAN, YELLOW, WHITE
 //}; 

//Original video Tiempo 18503
//Medio Tiempo 17295
//Optimizado Tiempo 10649
//Optimizado final Tiempo 8340
#endif
