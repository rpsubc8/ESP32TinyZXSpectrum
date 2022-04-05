#ifndef GB_SNA_H
 #define GB_SNA_H

 
 #include "sna/snaDiag48K.h"
 #include "sna/snaFantasy48K.h"
 #include "sna/snaSPpong48K.h"
 #include "sna/snaManic48K.h"
 #include "sna/snaFire48K.h"
 //#include "sna/snaSilkwormKM48K.h"
 #include "sna/sna3Dcoolkm48K.h"
 #include "sna/snaAT4OPENKM48K.h"
 //#include "sna/snaBREDITORKM48K.h"
 #include "sna/snaSIPKA48K.h"
 #include "sna/snaMouseTestZxds48K.h"
 //#include "sna/snaArkanoid48K.h" 
 #include "sna/snaFireKey.h" 
 #include "sna/snaTritone1.h" 
 //error in naming "sna/snaZ80full48k.h", was written with capital "48K" before
 //In file included from ZXESPectrum/Disk.cpp:4:0:
 //ZXESPectrum/dataFlash/gbsna.h:20:32: fatal error: sna/snaZ80full48k.h: No such file or directory
  #include "sna/snaZ80full48k.h"

 #include "sna/snaZ80full48k.h"


 //#define max_list_sna_48 10
 
 #define max_list_sna_48 12
 
//#define max_list_sna_48 0
 
 //#define max_list_sna_48 2
// #define max_list_sna_48 1 
 
 //sna 48K 
 //Titulos
 static const char * gb_list_sna_48k_title[max_list_sna_48]={
   
  "diag",
  "fantasy",
  "sppong",
  "manic",
  "fire",  
  //"Arkanoid",
  //"SilkWormKM",
  "3DcoolKm",
  "AT4openKM",
  //"BrEditorKM",
  "Sipka",
  "MouseTestZxds",
  "FireKey",
  "Tritone1",
  "Z80FULL"
  //"EightOfNoise",
  //"WWTF"
 };
 
 //Datos 48K sna
 static const unsigned char * gb_list_sna_48k_data[max_list_sna_48]={
   
  gb_sna_diag_48k,
  gb_sna_fantasy_48k,
  gb_sna_sppong_48k,
  gb_sna_manic_48k,
  gb_sna_fire_48k,
  //gb_sna_arkanoid_48k,
  //gb_sna_SILKWORMKM_48k,
  gb_3dcoolkm_48k,
  gb_sna_AT4OPENKM_48k,
  //gb_sna_BREDITORKM_48k,
  gb_sna_SIPKA_48K_H_48k,
  gb_sna_mouseTestZxds_48k,
  gb_sna_FireKey_48k,
  gb_sna_Tritone1_48k,
  gb_sna_Z80FULL_48K
//  gb_sna_eightOfNoise_48k,
//  gb_sna_wwtf_48k

 };


 //#include "sna/snaMusic2tap128.h"
 #include "sna/snaMusicDemo128.h"
 
 //#include "sna/snaAbadia128K.h"
 //#include "sna/snaRobocop128K.h"
 //#include "sna/snaRobocop2128K.h"

 //#define max_list_sna_128 5
 #define max_list_sna_128 1
 

 //#define max_list_sna_128 0

 //sna 128K
 //Titulos
 static const char * gb_list_sna_128k_title[max_list_sna_128]={  
  //"Music2tap128",
  "MusicDemo128"
  //,"Abadia",
  //"Robocop",
  //"Robocop2"
 };

 //Datos 128K
 static const unsigned char * gb_list_sna_128k_data[max_list_sna_128]={  
  //gb_sna_Music2tap128_128k,
  gb_sna_MusicDemo128_128k
  //,gb_sna_abadia_128k,
  //gb_sna_robocop_128k,
  //gb_sna_robocop2_128k
 };

#endif 
