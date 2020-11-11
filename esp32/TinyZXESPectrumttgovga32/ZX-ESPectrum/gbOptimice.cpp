#include "gbOptimice.h"
#include "def/hardware.h"
#include <Arduino.h>

 //extern unsigned char gb_cache_zxcolor[8];
 
 unsigned char gb_cache_zxcolor[8]=
 {//cache de colores optimizado
  BLACK, BLUE, RED, MAGENTA, GREEN, CYAN, YELLOW, WHITE
 };

 #ifdef use_lib_use_bright
  unsigned char gb_cache_zxcolor_bright[8]=
  {//cache de colores optimizado
   BRI_BLACK, BRI_BLUE, BRI_RED, BRI_MAGENTA, BRI_GREEN, BRI_CYAN, BRI_YELLOW, BRI_WHITE
  };
 #endif 

 //unsigned char gb_cache_zxcolor[8]=
 //{//cache de colores optimizado
 // BLACK, BLUE, RED, MAGENTA, GREEN, CYAN, YELLOW, WHITE
 //};