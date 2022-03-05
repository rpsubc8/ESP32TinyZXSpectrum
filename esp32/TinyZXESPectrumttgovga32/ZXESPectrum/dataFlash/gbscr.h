#ifndef GB_SCR_H
 #define GB_SCR_H

 #include <stddef.h>
 
 #include "scr/scr4x4Screen48k.h"
 


 #define max_list_scr_48 1
 
 //#define max_list_scr_48 0
 
 
 //screen 48K 
 //Titulos
 static const char * gb_list_scr_48k_title[max_list_scr_48]={     
  "4x4"  
 };
 
 //Datos 48K
 static const unsigned char * gb_list_scr_48k_data[max_list_scr_48]={     
  gb_scr_4x4_48k  
 };

#endif
