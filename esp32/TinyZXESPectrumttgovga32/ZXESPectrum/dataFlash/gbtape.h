#ifndef GB_TAPE_H
 #define GB_TAPE_H

 #include <stddef.h>
 
 #include "tape/tape3dgraph19xx48k.h"
 #include "tape/tape4x4Screen48k.h"
 
 
 //#include "tape/tape1942game48k.h"
 //#include "tape/tape3dlunagame.h"
 
 //#define max_list_tape_48 4
 
 #define max_list_tape_48 2
 #define max_list_tape_128 2
 
 

 //#define max_list_tape_48 0
 //#define max_list_tape_128 0
 
 
 
 //tapes 48K 
 //Titulos
 static const char * gb_list_tapes_48k_title[max_list_tape_48]={     
     
  "3D",
  "4x4"//,
  //"1942",
  //"Luna Attack"  
  
 };

 //Tamanio en bytes
 static const int gb_list_tapes_48k_size[max_list_tape_48]={     
     
  2272,
  7167//,
  //47378,
  //21776  
  
 }; 
 
 //Datos 48K
 static const unsigned char * gb_list_tapes_48k_data[max_list_tape_48]={     
     
  gb_tape_3dgraph19xx_48k,
  gb_tape_4x4screen_48k//,
  //gb_tape_game1942_48k,
  //gb_tape_game3dlunaAttack_48k  
  
 };

 //Datos 128K
 static const char * gb_list_tapes_128k_title[max_list_tape_128]={     
     
  "3D",
  "4x4"
    
 };

  //Datos 128K
 static const unsigned char * gb_list_tapes_128k_data[max_list_tape_128]={     
     
  gb_tape_3dgraph19xx_48k,
  gb_tape_4x4screen_48k  
  
 };

#endif
