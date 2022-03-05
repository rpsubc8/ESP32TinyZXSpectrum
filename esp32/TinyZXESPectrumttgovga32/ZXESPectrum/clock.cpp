#include "gbConfig.h"
#ifdef use_lib_core_linkefong


 #include "gbGlobals.h"
 #include "Disk.h"
 #include "speed.h"
 #include <Arduino.h>

 int CalcTStates() 
 {
  //if (cfg_arch == "48K") 
  if (gb_cfg_arch_is48K == 1)
  {
   return CPU_SPEED_MHZ_ZX48 * FRAME_PERIOD_MS * 1000;
  } 
  else
  {
   return CPU_SPEED_MHZ_ZX128 * FRAME_PERIOD_MS * 1000;
  }
 }

#endif