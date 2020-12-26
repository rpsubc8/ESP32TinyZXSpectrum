#include "gbOptimice.h"
#include "dataFlash/gbtape.h"
#include "dataFlash/gbscr.h"
#include "dataFlash/gbrom.h"
#include "dataFlash/gbsna.h"
#include "osd.h"
#include "Disk.h"
#include "Emulator/Keyboard/PS2Kbd.h"
#include "Emulator/z80main.h"
#include "ZX-ESPectrum.h"
#include "def/Font.h"
#include "def/keys.h"
#include "def/hardware.h"
#include "gb_globals.h"

const unsigned char gb_const_monoBlue8[8]=
{//cache de colores optimizado
 0,16,32,36,48,56,60,62
};
const unsigned char gb_const_monoGreen8[8]=
{//cache de colores optimizado
 0,4,8,12,14,24,28,30
};
const unsigned char gb_const_monoRed8[8]=
{//cache de colores optimizado
 0,1,2,3,6,7,11,27
};
const unsigned char gb_const_grey8[8]=
{//escala de grises
 0,16,21,33,37,42,58,63
};
const unsigned char gb_const_colornormal8[8]=
{
  BLACK, BLUE, RED, MAGENTA, GREEN, CYAN, YELLOW, WHITE
};

#ifdef use_lib_use_bright
 const unsigned char gb_const_monoBlue8_bright[8]=
 {//cache de colores optimizado
  0,16,32,36,48,56,60,62
 };
 const unsigned char gb_const_monoGreen8_bright[8]=
 {//cache de colores optimizado
  0,4,8,12,14,24,28,30
 };
 const unsigned char gb_const_monoRed8_bright[8]=
 {//cache de colores optimizado
  0,1,2,3,6,7,11,27
 };
 const unsigned char gb_const_grey8_bright[8]=
 {//escala de grises
  0,16,21,33,37,42,58,63
 };
 const unsigned char gb_const_colornormal8_bright[8]=
 {
   BRI_BLACK, BRI_BLUE, BRI_RED, BRI_MAGENTA, BRI_GREEN, BRI_CYAN, BRI_YELLOW, BRI_WHITE
 };
#endif

extern int gb_screen_xIni;
extern int gb_screen_yIni;
extern unsigned char gb_cache_zxcolor[8];
#ifdef use_lib_use_bright
 extern unsigned char gb_cache_zxcolor_bright[8];
#endif
static unsigned char gb_show_osd_main_menu=0;


void SDLOSDClear()
{
 //SDLClear(gb_osd_sdl_surface);
}
#define max_gb_osd_screen 7
const char * gb_osd_screen[max_gb_osd_screen]={
 "Cols Left",
 "Pixels Top",
 "Color 8",
 "Mono Blue 8",
 "Mono Green 8",
 "Mono Red 8",
 "Mono Grey 8" 
};

#define max_gb_osd_screen_values 5
const char * gb_osd_screen_values[max_gb_osd_screen_values]={
 "4",
 "3",
 "2", 
 "1", 
 "0"
};

#define max_gb_osd_speed_div_emulate 8
const char * gb_osd_speed_div_emulate[max_gb_osd_speed_div_emulate]={
 "5",
 "2",
 "4",
 "8",
 "16",
 "32",
 "64",
 "0"
};

#define max_gb_main_menu 9
const char * gb_main_menu[max_gb_main_menu]={
 "Load SNA",
 "Select ROM",
 "Load SCR TAP",
 "Select TAP",
 "Load SCR",
 "Speed",
 "Screen Adjust",
 "Reset",
 "Return"
};

#define max_gb_machine_menu 2
const char * gb_machine_menu[max_gb_machine_menu]={
 "48K",
 "128K"
};

#define max_gb_speed_menu 5
const char * gb_speed_menu[max_gb_speed_menu]={
 "Normal",
 "2x",
 "4x",
 "8x",
 "16x"
};

#define max_gb_speed_videoaudio_options_menu 5
const char * gb_speed_videoaudio_options_menu[max_gb_speed_videoaudio_options_menu]={
 "Audio poll",
 "Video delay",
 "Video speed",
 "Keyboard poll",
 "Div Delay Emul"
};

#define max_gb_speed_sound_menu 9
const char * gb_speed_sound_menu[max_gb_speed_sound_menu]={
 "0",
 "1",
 "2",
 "3",
 "4",
 "5",
 "6",
 "7",
 "8"
};

#define max_gb_reset_menu 2
const char * gb_reset_menu[max_gb_reset_menu]={
 "Soft",
 "Hard"
};

//****************************
//void SDLEsperaTeclado()
//{
// //SDL_WaitEvent(gb_osd_sdl_event);
//}

#ifdef use_lib_vga360x200
 #define gb_pos_x_menu 120
#else
 #define gb_pos_x_menu 80
#endif 
#define gb_pos_y_menu 50
#define gb_osd_max_rows 10

void OSDMenuRowsDisplayScroll(const char **ptrValue,unsigned char currentId,unsigned char aMax)
{//Dibuja varias lineas
 vga.setTextColor(WHITE,BLACK);
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
  vga.print("                    ");
 }
 
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  if (currentId >= aMax)
   break;
  if (i == 0)
   vga.setTextColor(CYAN,BLUE);
  else
   vga.setTextColor(WHITE,BLACK);
  vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
  vga.print(ptrValue[currentId]);
  currentId++;
 }     
}

//Maximo 256 elementos
unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax)
{
 unsigned char aReturn=0;
 unsigned char salir=0;  
 #ifdef use_lib_vga320x240
  vga.clear(BLACK);
  vga.fillRect(0,0,320,240,BLACK);
  vga.fillRect(0,0,320,240,BLACK);//Repeat Fix visual defect   
 #else
  #ifdef use_lib_vga320x200
   vga.clear(BLACK);
   vga.fillRect(0,0,320,200,BLACK);
   vga.fillRect(0,0,320,200,BLACK);//Repeat Fix visual defect 
  #else
   #ifdef use_lib_vga360x200
    vga.clear(BLACK);
    vga.fillRect(0,0,360,200,BLACK);
    vga.fillRect(0,0,360,200,BLACK);//Repeat Fix visual defect 
   #endif
  #endif  
 #endif
 vTaskDelay(2);
 vga.setTextColor(WHITE,BLACK);
 vga.setCursor((gb_pos_x_menu-(32)), gb_pos_y_menu-16); 
 vga.print("Mod ZXESPectrum by Ackerman");
 vga.setTextColor(BLACK,WHITE);
 //SDLClear(gb_osd_sdl_surface);  
 for (int i=0;i<20;i++)
 { 
  //SDLprintChar(gb_osd_sdl_surface,' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,BLACK,WHITE,1);
  vga.setCursor((gb_pos_x_menu+(i*6)), gb_pos_y_menu);
  vga.print(" ");
 }
 //SDLprintText(gb_osd_sdl_surface,cadTitle,gb_pos_x_menu,gb_pos_y_menu,BLACK,WHITE,1);
 vga.setCursor(gb_pos_x_menu,gb_pos_y_menu);
 vga.print(cadTitle);  
 //for (int i=0;i<aMax;i++)
 //{     
 // //SDLprintText(gb_osd_sdl_surface,ptrValue[i],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK),1);  
 // vga.setTextColor(((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK));
 // vga.setCursor(gb_pos_x_menu,(gb_pos_y_menu+8+(i<<3)));
 // vga.print(ptrValue[i]);
 //}
 OSDMenuRowsDisplayScroll(ptrValue,0,aMax);
 //SDL_Flip(gb_osd_sdl_surface); 
 while (salir == 0)
 {
  //SDL_PollEvent(gb_osd_sdl_event);
  //if(SDL_WaitEvent(gb_osd_sdl_event))
  {
   //if(gb_osd_sdl_event->type == SDL_KEYDOWN)
   {
    //switch(gb_osd_sdl_event->key.keysym.sym)
    {
     //case SDLK_UP:
     if (checkAndCleanKey(KEY_CURSOR_UP))
     {
      if (aReturn>0) aReturn--;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkAndCleanKey(KEY_CURSOR_DOWN))
     {//SDLK_DOWN
      if (aReturn < (aMax-1)) aReturn++;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkAndCleanKey(KEY_ENTER))
     {
      salir= 1;
     }
     //case SDLK_KP_ENTER: case SDLK_RETURN: salir= 1;break;
     if (checkAndCleanKey(KEY_ESC))
     {
      salir=1; aReturn= 255;    
     }
     //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
     //default: break;
    }
    //SDL_Flip(gb_osd_sdl_surface);
    //SDL_PollEvent(gb_osd_sdl_event);
   }  
  }  
 }
 gb_show_osd_main_menu= 0;
 return aReturn;
}

//Menu SNA
void ShowTinySNAMenu()
{
 unsigned char aSelNum;     
 aSelNum = ShowTinyMenu("Machine SNA",gb_machine_menu,max_gb_machine_menu);
 Z80EmuSelectTape(0);
 if (aSelNum == 0)
 {
  aSelNum = ShowTinyMenu("48K SNA",gb_list_sna_48k_title,max_list_sna_48);                
  //strcpy(cfg_arch,"48K");
  //cfg_arch = "48K";
  //gb_cfg_arch_is48K = 1;
  changeSna2Flash(aSelNum);
 }
 else
 {
  aSelNum = ShowTinyMenu("128K SNA",gb_list_sna_48k_title,max_list_sna_48);                
  //strcpy(cfg_arch,"128K");
  //cfg_arch = "128K";
  //gb_cfg_arch_is48K = 0;
  changeSna2Flash(aSelNum);
 }
 vTaskDelay(2);
}

//Menu ROM
void ShowTinyROMMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("Machine ROM",gb_machine_menu,max_gb_machine_menu);
 Z80EmuSelectTape(0);
 if (aSelNum == 0)
 {
  aSelNum = ShowTinyMenu("48K ROM",gb_list_roms_48k_title,max_list_rom_48);
  //strcpy(cfg_arch,"48K");  
  //cfg_arch = "48K";
  gb_cfg_arch_is48K = 1;
  load_rom2flash(1,aSelNum);  
 }
 else
 {
  aSelNum = ShowTinyMenu("128K ROM",gb_list_roms_128k_title,max_list_rom_128);        
  //strcpy(cfg_arch,"128K");
  //cfg_arch = "128K";
  gb_cfg_arch_is48K = 0;
  load_rom2flash(0,aSelNum);  
 }
 vTaskDelay(2);
 //SDL_Delay(2);
 zx_reset();     
}

//Menu TAPE
void ShowTinyTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 load_tape2Flash(aSelNum);
}

//Play sound tape
void ShowTinySelectTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 Z80EmuSelectTape(aSelNum);
}

//Menu SCREEN
void ShowTinySCRMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K SCREEN",gb_list_scr_48k_title,max_list_scr_48);
 load_scr2Flash(aSelNum);
}

//Menu velocidad emulador
void ShowTinySpeedMenu()
{
 unsigned char aSelNum,aSelNumSpeed;
 aSelNum = ShowTinyMenu("SPEED VIDEO AUDIO",gb_speed_videoaudio_options_menu,max_gb_speed_videoaudio_options_menu);
 if (aSelNum == 255)
  return;
 switch (aSelNum)
 {
  case 0: aSelNumSpeed= ShowTinyMenu("AUDIO Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
   gb_current_ms_poll_sound= aSelNumSpeed;
   break;
  case 1: aSelNumSpeed= ShowTinyMenu("Video DELAY ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
   gb_current_delay_emulate_ms = aSelNumSpeed;
   break;
  case 2: //gb_current_frame_crt_skip = ShowTinyMenu("Skip Frame",gb_value_binary_menu,max_gb_value_binary_menu);
   aSelNumSpeed = ShowTinyMenu("SPEED",gb_speed_menu,max_gb_speed_menu);
   #ifdef use_lib_vga_thread
    switch (aSelNumSpeed)
    {
     case 0: gbFrameSkipVideoMaxCont = 0; break;
     case 1: gbFrameSkipVideoMaxCont = 1; break;
     case 2: gbFrameSkipVideoMaxCont = 2; break;
     case 3: gbFrameSkipVideoMaxCont = 4; break;
     case 4: gbFrameSkipVideoMaxCont = 8; break;
    }
   #else
    switch (aSelNumSpeed)
    {
     case 0: gbDelayVideo = 20; break;
     case 1: gbDelayVideo = 25; break;
     case 2: gbDelayVideo = 30; break;
     case 3: gbDelayVideo = 40; break;
     case 4: gbDelayVideo = 50; break;
    } 
   #endif
   break;
  case 3: aSelNumSpeed= ShowTinyMenu("Keyboard Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
   gb_current_ms_poll_keyboard= aSelNumSpeed;
   break;
  case 4: aSelNumSpeed= ShowTinyMenu("Delay Div Emul",gb_osd_speed_div_emulate,max_gb_osd_speed_div_emulate);
   switch (aSelNumSpeed)
   {
    case 0: gb_current_delay_emulate_div_microsec= 5; break;
    case 1: gb_current_delay_emulate_div_microsec= 2; break;
    case 2: gb_current_delay_emulate_div_microsec= 4; break;    
    case 3: gb_current_delay_emulate_div_microsec= 8; break;
    case 4: gb_current_delay_emulate_div_microsec= 16; break;    
    case 5: gb_current_delay_emulate_div_microsec= 32; break;
    case 6: gb_current_delay_emulate_div_microsec= 64; break;
    case 7: gb_current_delay_emulate_div_microsec= 0; break;
   }
   break;   
  default: break;
 }
}

//Ajustar pantalla
void ShowTinyScreenAdjustMenu()
{
 unsigned char aSelNum, auxCol; 
 aSelNum= ShowTinyMenu("Screen Adjust",gb_osd_screen,max_gb_osd_screen);
 switch (aSelNum)
 { 
  case 2:
   //OSD_PreparaPaleta8Colores();
   memcpy(gb_cache_zxcolor,gb_const_colornormal8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_colornormal8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 3:
   //OSD_PreparaPaleta64para8Colores();
   memcpy(gb_cache_zxcolor,gb_const_monoBlue8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoBlue8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 4:
   //OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_monoGreen8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoGreen8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 5:
   //OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_monoRed8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoRed8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
  case 6:
   //OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_grey8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_grey8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;          
 } 
 auxCol= ShowTinyMenu("Cols or pixels",gb_osd_screen_values,max_gb_osd_screen_values); 
 switch (aSelNum)
 {
  case 0: gb_screen_xIni = -(auxCol<<3); break;
  case 1: gb_screen_yIni = (auxCol<=3)?(-auxCol):(-3); break;
  default: break;
 }
 //Serial.printf("gb_screen_yIni %d %d\n",gb_screen_yIni,auxCol);
 //Serial.printf("gb_screen_yIni %d \n",gb_screen_yIni); 
}

//Menu resetear
void ShowTinyResetMenu()
{
 unsigned char aSelNum;
 aSelNum= ShowTinyMenu("Reset",gb_reset_menu,max_gb_reset_menu);   
 Z80EmuSelectTape(0);
 zx_reset();
 if (aSelNum == 1)
  ESP.restart();
}


//*******************************************
void SDLActivarOSDMainMenu()
{     
 gb_show_osd_main_menu= true;   
}

//Very small tiny osd
void do_tinyOSD() 
{
 unsigned char aSelNum;
// SDLprintText("Ackerman Mod ZXESP32 Win32 Emulator",10,250,BLACK,WHITE);
 if (checkAndCleanKey(KEY_F1))
 {
  #ifdef use_lib_sound_ay8912
   ResetSound();
   gb_silence_all_channels = 1;
  #endif  
  gb_show_osd_main_menu= 1;
  return;
 }

 if (gb_show_osd_main_menu == 1)
 {
  aSelNum = ShowTinyMenu("MAIN MENU",gb_main_menu,max_gb_main_menu);
  switch (aSelNum)
  {
   case 0: ShowTinySNAMenu(); break;
   case 1: ShowTinyROMMenu(); break;
   case 2: ShowTinyTAPEMenu(); break;
   case 3: ShowTinySelectTAPEMenu(); break;
   case 4: ShowTinySCRMenu(); break;
   case 5: ShowTinySpeedMenu(); break;
   case 6: ShowTinyScreenAdjustMenu(); break;
   case 7: ShowTinyResetMenu(); break;
   default: break;
  }
 }
 #ifdef use_lib_sound_ay8912
  gb_silence_all_channels = 0;
 #endif
}
