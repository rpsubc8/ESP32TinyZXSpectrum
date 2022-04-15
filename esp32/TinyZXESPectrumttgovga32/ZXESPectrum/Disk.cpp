#include "gbConfig.h"
#include "gbGlobals.h"
#include "dataFlash/gbtape.h"
#ifndef use_lib_wifi
 #include "dataFlash/gbscr.h"
#endif 
#include "dataFlash/gbrom.h"
#ifndef use_lib_wifi
 #include "dataFlash/gbsna.h"
#endif 
#include "PS2Kbd.h"
#include "Memory.h"
#include "z80main.h"
#include "ZXESPectrum.h"
//#include "def/ascii.h"
//#include "types.h"
#include "osd.h"
#include "Disk.h"

#ifdef use_lib_wifi
 #include "gbWifiConfig.h"
 #include "gbWifi.h"
#endif

//void IRAM_ATTR kb_interruptHandler(void);
//void zx_reset();

// Globals
//unsigned char gb_cfg_arch_is48K = 1; //Modo 48k

//void SetMode48K()
//{
// rom_latch = 0;
// rom_in_use = 0;
// bank_latch = 0;
// paging_lock = 1;
//}

//void SetMode128K()
//{
// rom_in_use = 1;
// rom_latch = 1;
// paging_lock = 1;
// bank_latch = 0;
// video_latch = 0;
// paging_lock = 1;
// bank_latch = 0;
// video_latch = 0;   
//}


#ifdef use_lib_wifi
#else
 //Lee una pantalla a zona video
 void load_scr2Flash(unsigned char id)
 {
 // for (int i=0;i<6912;i++)
 //  writebyte((16384+i),gb_list_scr_48k_data[id][i]); //screen
  #ifdef use_lib_core_linkefong
   memcpy(ram5,&gb_list_scr_48k_data[id][0],6912); //optimizado
  #else 
   #ifdef use_lib_core_jsanchezv
    memcpy(ram5_jsanchezv,&gb_list_scr_48k_data[id][0],6912); //optimizado
   #endif
  #endif
 
 }
#endif


void LoadTapeName(unsigned char id,char *cad,int * cont)
{for (unsigned char i=0;i<10;i++) 
 {
  cad[i]= gb_list_tapes_48k_data[id][*cont];
  *cont = *cont+1;
 }
 cad[10]='\0'; 
}

unsigned char LoadTapeByte(unsigned char id,int * cont)
{unsigned char aReturn = gb_list_tapes_48k_data[id][*cont];
 *cont = *cont + 1;
 return aReturn;
}

unsigned short int LoadTapeWord(unsigned char id,int * cont)
{unsigned short int aReturn = (gb_list_tapes_48k_data[id][*cont]) | (gb_list_tapes_48k_data[id][(*cont)+1]<<8);
 *cont = *cont + 2;
 return aReturn;    
}

unsigned char SearchTapeByte(unsigned char id,unsigned char aData,int * cont)
{unsigned char salir=0;
 while (salir == 0)
 {
  if ((gb_list_tapes_48k_data[id][*cont] != aData) && ((*cont)<gb_list_tapes_48k_size[id]))
   *cont = *cont + 1;
  else
   salir = 1;
 }
 return salir;
}

//Devuelve el numero de bloques y lista con el contador del bloque
int GetListBlocks(unsigned char id,int *auxList)
{
}

#ifdef use_lib_core_linkefong
//Lee una cinta a memoria
void load_tape2Flash(unsigned char id)
{
 int cont=2;
 char cad_name[11]="";
 int auxLen,auxParam1,auxParam2,auxLenVar,auxLenScreen;
 int auxFlag,auxCRC,auxSize,auxPgrSize;
 int contHead;
 unsigned char tipo;
 char cad_dest[255];
 int x_ini=5;
 int y_ini=440; 

 #ifdef use_lib_sound_ay8912
  ResetSound();
 #endif 
 
 //SDLOSDClear();

 
 //0 Programa
 //1 Array de números
 //2 Array de caracteres
 //3 CODE (datos a cargar en memoria)
 cont=2;
 auxFlag = LoadTapeByte(id,&cont);
 tipo = LoadTapeByte(id,&cont);
 LoadTapeName(id,cad_name,&cont);
 auxLen = LoadTapeWord(id,&cont); //Longitud datablock
 auxParam1 = LoadTapeWord(id,&cont);
 auxParam2 = LoadTapeWord(id,&cont);
 auxCRC = LoadTapeByte(id,&cont); 
 
 sprintf(cad_dest,"F%d T%d %sL%d A%d B%d C%d",auxFlag,tipo,cad_name,auxLen,auxParam1,auxParam2,auxCRC);
 //SDLprintText(SDLOSDGetSurface(),cad_dest,x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;
 
 if (tipo != 0)
  return;
 
 cont += auxLen;
 SearchTapeByte(id,0x13,&cont);
 cont += 2;
 auxFlag = LoadTapeByte(id,&cont);
 tipo = LoadTapeByte(id,&cont);
 LoadTapeName(id,cad_name,&cont);
 auxLen = LoadTapeWord(id,&cont); //Longitud datablock
 auxParam1 = LoadTapeWord(id,&cont);
 auxParam2 = LoadTapeWord(id,&cont);
 auxCRC = LoadTapeByte(id,&cont);
 sprintf(cad_dest,"F%d T%d %sL%d A%d B%d C%d",auxFlag,tipo,cad_name,auxLen,auxParam1,auxParam2,auxCRC);
 //SDLprintText(SDLOSDGetSurface(),cad_dest,x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;
 
 if (tipo != 3)
  return;
  
 SearchTapeByte(id,0xFF,&cont);
 cont++;
 if ((auxParam2 >= 32768)&&(auxParam1 == 16384)&&(auxLen == 6912)) 
 {
  //SDLprintText(SDLOSDGetSurface(),"SCREEN FILE",x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;
  #ifdef use_lib_core_linkefong
   memcpy(ram5,&gb_list_tapes_48k_data[id][cont],6912); //optimizado
  #else
   #ifdef use_lib_core_jsanchezv
    memcpy(ram5_jsanchezv,&gb_list_tapes_48k_data[id][cont],6912); //optimizado
   #endif 
  #endif 
 }
 else
 {
  if ((auxParam1 == 16384)&&(auxLen == 6912))
  {
   //SDLprintText(SDLOSDGetSurface(),"POSIBLE SCREEN GAME",x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;
   #ifdef use_lib_core_linkefong   
    memcpy(ram5,&gb_list_tapes_48k_data[id][cont],6912); //optimizado
   #else
    #ifdef use_lib_core_jsanchezv
     memcpy(ram5_jsanchezv,&gb_list_tapes_48k_data[id][cont],6912); //optimizado
    #endif
   #endif 

   cont += auxLen;
   SearchTapeByte(id,0x13,&cont);
   sprintf(cad_dest,"cont %d",cont);
   //SDLprintText(SDLOSDGetSurface(),cad_dest,x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;   
   cont += 2;
   auxFlag = LoadTapeByte(id,&cont);
   tipo = LoadTapeByte(id,&cont);
   LoadTapeName(id,cad_name,&cont);
   auxLen = LoadTapeWord(id,&cont); //Longitud datablock
   auxParam1 = LoadTapeWord(id,&cont);
   auxParam2 = LoadTapeWord(id,&cont);
   auxCRC = LoadTapeByte(id,&cont);
   sprintf(cad_dest,"F%d T%d %sL%d A%d B%d C%d",auxFlag,tipo,cad_name,auxLen,auxParam1,auxParam2,auxCRC);
   //SDLprintText(SDLOSDGetSurface(),cad_dest,x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;
   
   SearchTapeByte(id,0xFF,&cont);
   cont++;
   sprintf(cad_dest,"cont %d",cont);
   //SDLprintText(SDLOSDGetSurface(),cad_dest,x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;
   
   for (int i=0;i<auxLen;i++)
   {
    writebyte(auxParam1,gb_list_tapes_48k_data[id][(cont+i)]);
   }
   
    //if (strcmp(cfg_arch,"48K") == 0)
    //if (cfg_arch == "48K")
    if (gb_cfg_arch_is48K == 1)
    {
        rom_latch = 0;
        rom_in_use = 0;
        bank_latch = 0; gb_ptr_IdRomRam[3]= 0;
        paging_lock = 1;
    }   
   
   //_zxCpu.pc = auxParam1-1;
   //_zxCpu.pc = 25000;
  }
  else
  {
   //SDLprintText(SDLOSDGetSurface(),"OTHER FILE",x_ini,y_ini,gb_cache_zxcolor[id_cache_WHITE],gb_cache_zxcolor[id_cache_BLACK],0); y_ini+= 8;
  }
 } 
}
#endif

#ifdef use_lib_core_linkefong
 #ifdef use_lib_wifi
 #else
  //Prueba Load 128 SNA
  void load_ram2Flash128(unsigned char id)
  {
   int contBuffer=0;
   //uint16_t size_read;
   byte sp_h, sp_l;
   uint16_t retaddr;
   //int sna_size;
   #ifdef use_lib_sound_ay8912
    ResetSound();
   #endif
   if (id >= max_list_sna_128)
    return;
   zx_reset();

    #ifdef use_lib_only_48k
     memset(ram0,0,0x4000);
     memset(ram2,0,0x4000);
     memset(ram5,0,0x4000);
    #else
     memset(ram0,0,0x4000);
     memset(ram1,0,0x4000);
     memset(ram2,0,0x4000);
     memset(ram3,0,0x4000);
     memset(ram4,0,0x4000);
     memset(ram5,0,0x4000);
     memset(ram6,0,0x4000);
     memset(ram7,0,0x4000);
    #endif   

    // Read in the registers    
    _zxCpu.i = gb_list_sna_128k_data[id][0]; //lhandle.read();
    _zxCpu.registers.byte[Z80_L] = gb_list_sna_128k_data[id][1];//lhandle.read();
    _zxCpu.registers.byte[Z80_H] = gb_list_sna_128k_data[id][2];//lhandle.read();
    _zxCpu.registers.byte[Z80_E] = gb_list_sna_128k_data[id][3];//lhandle.read();
    _zxCpu.registers.byte[Z80_D] = gb_list_sna_128k_data[id][4];//lhandle.read();
    _zxCpu.registers.byte[Z80_C] = gb_list_sna_128k_data[id][5];//lhandle.read();
    _zxCpu.registers.byte[Z80_B] = gb_list_sna_128k_data[id][6];//lhandle.read();
    _zxCpu.registers.byte[Z80_F] = gb_list_sna_128k_data[id][7];//lhandle.read();
    _zxCpu.registers.byte[Z80_A] = gb_list_sna_128k_data[id][8];//lhandle.read();

    _zxCpu.alternates[Z80_HL] = _zxCpu.registers.word[Z80_HL];
    _zxCpu.alternates[Z80_DE] = _zxCpu.registers.word[Z80_DE];
    _zxCpu.alternates[Z80_BC] = _zxCpu.registers.word[Z80_BC];
    _zxCpu.alternates[Z80_AF] = _zxCpu.registers.word[Z80_AF];

    _zxCpu.registers.byte[Z80_L] = gb_list_sna_128k_data[id][9];//lhandle.read();
    _zxCpu.registers.byte[Z80_H] = gb_list_sna_128k_data[id][10];//lhandle.read();
    _zxCpu.registers.byte[Z80_E] = gb_list_sna_128k_data[id][11];//lhandle.read();
    _zxCpu.registers.byte[Z80_D] = gb_list_sna_128k_data[id][12];//lhandle.read();
    _zxCpu.registers.byte[Z80_C] = gb_list_sna_128k_data[id][13];//lhandle.read();
    _zxCpu.registers.byte[Z80_B] = gb_list_sna_128k_data[id][14];//lhandle.read();
    _zxCpu.registers.byte[Z80_IYL] = gb_list_sna_128k_data[id][15];//lhandle.read();
    _zxCpu.registers.byte[Z80_IYH] = gb_list_sna_128k_data[id][16];//lhandle.read();
    _zxCpu.registers.byte[Z80_IXL] = gb_list_sna_128k_data[id][17];//lhandle.read();
    _zxCpu.registers.byte[Z80_IXH] = gb_list_sna_128k_data[id][18];//lhandle.read();

    byte inter = gb_list_sna_128k_data[id][19];//lhandle.read();
    _zxCpu.iff2 = (inter & 0x04) ? 1 : 0;
    _zxCpu.r = gb_list_sna_128k_data[id][20];//lhandle.read();

    _zxCpu.registers.byte[Z80_F] = gb_list_sna_128k_data[id][21];//lhandle.read();
    _zxCpu.registers.byte[Z80_A] = gb_list_sna_128k_data[id][22];//lhandle.read();

    sp_l = gb_list_sna_128k_data[id][23];//lhandle.read();
    sp_h = gb_list_sna_128k_data[id][24];//lhandle.read();
    _zxCpu.registers.word[Z80_SP] = sp_l + sp_h * 0x100;

    _zxCpu.im = gb_list_sna_128k_data[id][25];//lhandle.read();
    byte bordercol = gb_list_sna_128k_data[id][26];//lhandle.read();

    borderTemp = bordercol;

    _zxCpu.iff1 = _zxCpu.iff2; 
 
 
    contBuffer = 27;
    //uint16_t buf_p = 0x4000;
        
    memcpy(ram5,&gb_list_sna_128k_data[id][contBuffer],0x4000);
    contBuffer+= 0x4000;
    memcpy(ram2,&gb_list_sna_128k_data[id][contBuffer],0x4000); //0x4000 + 17d
    contBuffer+= 0x8000; //Saltamos el banco cacheado 0x4000+0x4000
    
    byte retaddr_l = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read();
    byte retaddr_h = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read();
    retaddr = retaddr_l + retaddr_h * 0x100;
    byte tmp_port = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read();
    unsigned char auxBank= (tmp_port&0x07);
    //printf("port 0x7ffd %x banco:%d\n",tmp_port,auxBank);
    //fflush(stdout);
    
    byte tr_dos = gb_list_sna_128k_data[id][contBuffer++];//lhandle.read();
    byte tmp_latch = tmp_port & 0x7;
    bank_latch = tmp_latch; gb_ptr_IdRomRam[3] = bank_latch;
    
    contBuffer= 0x801B; //27+0x4000+0x4000
    switch (bank_latch)
    {     
     case 0: memcpy(ram0,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 1: memcpy(ram1,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 2: memcpy(ram2,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 3: memcpy(ram3,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 4: memcpy(ram4,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 5: memcpy(ram5,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 6: memcpy(ram6,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
     case 7: memcpy(ram7,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
    }
    contBuffer+= 0x4004; //0x4000+4    
    //131103 bytes de SNA 128                
    //Abadia banco 4 latch, luego 0, 1, 3, 6 y 7
    for (unsigned char i=0;i<8;i++)
    {
     if (i != 2 && i != 5 && i != bank_latch)
     {
      switch (i)
      {
       case 0: memcpy(ram0,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 1: memcpy(ram1,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 3: memcpy(ram3,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 4: memcpy(ram4,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 6: memcpy(ram6,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;
       case 7: memcpy(ram7,&gb_list_sna_128k_data[id][contBuffer],0x4000); break;                            
      }
      contBuffer+= 0x4000;           
     }
    }

    video_latch = bitRead(tmp_port, 3);
    rom_latch = bitRead(tmp_port, 4);
    paging_lock = bitRead(tmp_port, 5);
    bank_latch = tmp_latch; gb_ptr_IdRomRam[3] = tmp_latch;
    rom_in_use = rom_latch; 
        
    _zxCpu.pc = retaddr;
    //printf("SNA Ret address: %x\n",retaddr);
    //fflush(stdout);
    //printf("SNA Ret address: %x Stack: %x AF: %x Border: %x sna_size: %d rom: %d bank: %x\n", retaddr, _zxCpu.registers.word[Z80_SP], _zxCpu.registers.word[Z80_AF], borderTemp, sna_size, rom_in_use, bank_latch);
  }
 #endif
#endif

#ifdef use_lib_core_linkefong
 #ifdef use_lib_wifi
 #else
  //Lee SNA desde flash
  void load_ram2Flash(unsigned char id,unsigned char isSNA48K)
  {
   if (isSNA48K != 1)
   {
    load_ram2Flash128(id);
    return;
   }    
    int contBuffer=0;
    //JJ File lhandle;
    uint16_t size_read;
    byte sp_h, sp_l;
    uint16_t retaddr;
    //int sna_size;
    #ifdef use_lib_sound_ay8912
     ResetSound();
    #endif 

    if (id >= max_list_sna_48)
     return;

    zx_reset();

    //JJ Serial.printf("%s SNA: %ub\n", MSG_FREE_HEAP_BEFORE, ESP.getFreeHeap());

    KB_INT_STOP;

    #ifdef use_lib_only_48k
     memset(ram0,0,0x4000);
     memset(ram2,0,0x4000);
     memset(ram5,0,0x4000);
    #else
     memset(ram0,0,0x4000);
     memset(ram1,0,0x4000);
     memset(ram2,0,0x4000);
     memset(ram3,0,0x4000);
     memset(ram4,0,0x4000);
     memset(ram5,0,0x4000);
     memset(ram6,0,0x4000);
     memset(ram7,0,0x4000);
    #endif    

    //sna_size = 49179;       
    //Serial.println("Modo JJ "+cfg_arch);
    //if (cfg_arch == "48K") 
    if (gb_cfg_arch_is48K == 1)
    {
        rom_latch = 0;
        rom_in_use = 0;
        bank_latch = 0; gb_ptr_IdRomRam[3]=0;
        paging_lock = 1;
    }
    //if (sna_size < 50000 && cfg_arch != "48K") 
    //if (sna_size < 50000 && gb_cfg_arch_is48K != 1) 
    if (gb_cfg_arch_is48K != 1)
    {
        rom_in_use = 1;
        rom_latch = 1;
        paging_lock = 1;
        bank_latch = 0; gb_ptr_IdRomRam[3]=0;
        video_latch = 0;
        paging_lock = 1;
        bank_latch = 0; gb_ptr_IdRomRam[3]=0;
        video_latch = 0;
    }
    size_read = 0;
    // Read in the registers    
    _zxCpu.i = gb_list_sna_48k_data[id][0]; //lhandle.read();
    _zxCpu.registers.byte[Z80_L] = gb_list_sna_48k_data[id][1];//lhandle.read();
    _zxCpu.registers.byte[Z80_H] = gb_list_sna_48k_data[id][2];//lhandle.read();
    _zxCpu.registers.byte[Z80_E] = gb_list_sna_48k_data[id][3];//lhandle.read();
    _zxCpu.registers.byte[Z80_D] = gb_list_sna_48k_data[id][4];//lhandle.read();
    _zxCpu.registers.byte[Z80_C] = gb_list_sna_48k_data[id][5];//lhandle.read();
    _zxCpu.registers.byte[Z80_B] = gb_list_sna_48k_data[id][6];//lhandle.read();
    _zxCpu.registers.byte[Z80_F] = gb_list_sna_48k_data[id][7];//lhandle.read();
    _zxCpu.registers.byte[Z80_A] = gb_list_sna_48k_data[id][8];//lhandle.read();

    _zxCpu.alternates[Z80_HL] = _zxCpu.registers.word[Z80_HL];
    _zxCpu.alternates[Z80_DE] = _zxCpu.registers.word[Z80_DE];
    _zxCpu.alternates[Z80_BC] = _zxCpu.registers.word[Z80_BC];
    _zxCpu.alternates[Z80_AF] = _zxCpu.registers.word[Z80_AF];

    _zxCpu.registers.byte[Z80_L] = gb_list_sna_48k_data[id][9];//lhandle.read();
    _zxCpu.registers.byte[Z80_H] = gb_list_sna_48k_data[id][10];//lhandle.read();
    _zxCpu.registers.byte[Z80_E] = gb_list_sna_48k_data[id][11];//lhandle.read();
    _zxCpu.registers.byte[Z80_D] = gb_list_sna_48k_data[id][12];//lhandle.read();
    _zxCpu.registers.byte[Z80_C] = gb_list_sna_48k_data[id][13];//lhandle.read();
    _zxCpu.registers.byte[Z80_B] = gb_list_sna_48k_data[id][14];//lhandle.read();
    _zxCpu.registers.byte[Z80_IYL] = gb_list_sna_48k_data[id][15];//lhandle.read();
    _zxCpu.registers.byte[Z80_IYH] = gb_list_sna_48k_data[id][16];//lhandle.read();
    _zxCpu.registers.byte[Z80_IXL] = gb_list_sna_48k_data[id][17];//lhandle.read();
    _zxCpu.registers.byte[Z80_IXH] = gb_list_sna_48k_data[id][18];//lhandle.read();

    byte inter = gb_list_sna_48k_data[id][19];//lhandle.read();
    _zxCpu.iff2 = (inter & 0x04) ? 1 : 0;
    _zxCpu.r = gb_list_sna_48k_data[id][20];//lhandle.read();

    _zxCpu.registers.byte[Z80_F] = gb_list_sna_48k_data[id][21];//lhandle.read();
    _zxCpu.registers.byte[Z80_A] = gb_list_sna_48k_data[id][22];//lhandle.read();

    sp_l = gb_list_sna_48k_data[id][23];//lhandle.read();
    sp_h = gb_list_sna_48k_data[id][24];//lhandle.read();
    _zxCpu.registers.word[Z80_SP] = sp_l + sp_h * 0x100;

    _zxCpu.im = gb_list_sna_48k_data[id][25];//lhandle.read();
    byte bordercol = gb_list_sna_48k_data[id][26];//lhandle.read();

    borderTemp = bordercol;

    _zxCpu.iff1 = _zxCpu.iff2;

    //if (sna_size < 50000) 
    //{
        uint16_t thestack = _zxCpu.registers.word[Z80_SP];
        uint16_t buf_p = 0x4000;
        contBuffer = 27;
        //JJ while (lhandle.available()) {
        while (contBuffer< 50000)
        {
            //JJ writebyte(buf_p, lhandle.read());
            writebyte(buf_p, gb_list_sna_48k_data[id][contBuffer]);
            contBuffer++;
            buf_p++;
        }

        // uint16_t offset = thestack - 0x4000;
        // retaddr = ram5[offset] + 0x100 * ram5[offset + 1];
        retaddr = readword(thestack);
        #ifdef use_lib_log_serial
         Serial.printf("retaddr %x\n", retaddr);
        #endif
        _zxCpu.registers.word[Z80_SP]++;
        _zxCpu.registers.word[Z80_SP]++;
    //} 
    /*
    else 
    {
        uint16_t buf_p;
        for (buf_p = 0x4000; buf_p < 0x8000; buf_p++) 
        {
            //JJwritebyte(buf_p, lhandle.read());
            writebyte(buf_p, gb_list_sna_48k_data[id][contBuffer++]);
        }
        for (buf_p = 0x8000; buf_p < 0xc000; buf_p++) 
        {
            //JJwritebyte(buf_p, lhandle.read());            
            writebyte(buf_p, gb_list_sna_48k_data[id][contBuffer++]);
        }

        for (buf_p = 0xc000; buf_p < 0xffff; buf_p++) 
        {
            //JJwritebyte(buf_p, lhandle.read());
            writebyte(buf_p, gb_list_sna_48k_data[id][contBuffer++]);
        }

        byte machine_b = gb_list_sna_48k_data[id][contBuffer++];//lhandle.read();
        #ifdef use_lib_log_serial
         Serial.printf("Machine: %x\n", machine_b);
        #endif 
        byte retaddr_l = gb_list_sna_48k_data[id][contBuffer++];//lhandle.read();
        byte retaddr_h = gb_list_sna_48k_data[id][contBuffer++];//lhandle.read();
        retaddr = retaddr_l + retaddr_h * 0x100;
        byte tmp_port = gb_list_sna_48k_data[id][contBuffer++];//lhandle.read();

        byte tmp_byte;
        for (int a = 0xc000; a < 0xffff; a++) {
            bank_latch = 0; gb_ptr_IdRomRam[3]=0;
            tmp_byte = fast_readbyte(a);
            bank_latch = tmp_port & 0x07; gb_ptr_IdRomRam[3]=bank_latch;
            writebyte(a, tmp_byte);
        }

        byte tr_dos = gb_list_sna_48k_data[id][contBuffer++];//lhandle.read();
        byte tmp_latch = tmp_port & 0x7;
        for (int page = 0; page < 8; page++) {
            if (page != tmp_latch && page != 2 && page != 5) {
                bank_latch = page; gb_ptr_IdRomRam[3]= page;
                #ifdef use_lib_log_serial
                 Serial.printf("Page %d actual_latch: %d\n", page, bank_latch);
                #endif 
                for (buf_p = 0xc000; buf_p < 0xFFFF; buf_p++) 
                {
                    //JJwritebyte(buf_p, lhandle.read());
                    writebyte(buf_p, gb_list_sna_48k_data[id][contBuffer++]);                    
                }
            }
        }

        video_latch = bitRead(tmp_port, 3);
        rom_latch = bitRead(tmp_port, 4);
        paging_lock = bitRead(tmp_port, 5);
        bank_latch = tmp_latch; gb_ptr_IdRomRam[3]= tmp_latch;
        rom_in_use = rom_latch;
    }
    */
    //JJ lhandle.close();

    _zxCpu.pc = retaddr;
    //JJSerial.printf("%s SNA: %u\n", MSG_FREE_HEAP_AFTER, ESP.getFreeHeap());
    #ifdef use_lib_log_serial
    //Serial.printf("Ret address: %x Stack: %x AF: %x Border: %x sna_size: %d rom: %d bank: %x\n", retaddr,
    //              _zxCpu.registers.word[Z80_SP], _zxCpu.registers.word[Z80_AF], borderTemp, sna_size, rom_in_use,
    //              bank_latch);
    #endif                  
    KB_INT_START;
  }
 #endif 



 #ifdef use_lib_wifi
 void load_ram2FlashFromWIFI(char *cadUrl,unsigned char isSNA48K)
 {
  #ifdef use_lib_wifi_debug
   Serial.printf("loadram2FlashFromWIFI\n");
  #endif
  if (isSNA48K != 1)
  {
   //load_ram2Flash128(id);
   return;
  }    
    int contBuffer=0;    
    uint16_t size_read;
    byte sp_h, sp_l;
    uint16_t retaddr;    
    #ifdef use_lib_sound_ay8912
     ResetSound();
    #endif 

//JJ    if (id >= max_list_sna_48)
//JJ     return;

    zx_reset();    

    KB_INT_STOP;


    #ifdef use_lib_only_48k
     memset(ram0,0,0x4000);
     memset(ram2,0,0x4000);
     memset(ram5,0,0x4000);
    #else
     memset(ram0,0,0x4000);
     memset(ram1,0,0x4000);
     memset(ram2,0,0x4000);
     memset(ram3,0,0x4000);
     memset(ram4,0,0x4000);
     memset(ram5,0,0x4000);
     memset(ram6,0,0x4000);
     memset(ram7,0,0x4000);
    #endif

    if (gb_cfg_arch_is48K == 1)
    {
        rom_latch = 0;
        rom_in_use = 0;
        bank_latch = 0; gb_ptr_IdRomRam[3]=0;
        paging_lock = 1;
    }

    if (gb_cfg_arch_is48K != 1)
    {
        rom_in_use = 1;
        rom_latch = 1;
        paging_lock = 1;
        bank_latch = 0; gb_ptr_IdRomRam[3]=0;
        video_latch = 0;
        paging_lock = 1;
        bank_latch = 0; gb_ptr_IdRomRam[3]=0;
        video_latch = 0;
    }
    size_read = 0;

    #ifdef use_lib_wifi_debug
     Serial.printf("Check WIFI\n");
    #endif 
    if (Check_WIFI() == false)
    {
     return;
    }

    int leidos=0;    
    //Asignar_URL_stream_WIFI("http://192.168.0.36/zxspectrum/input/sna/48k/scene/Dream.sna");
    #ifdef use_lib_wifi_debug
     Serial.printf("URL:%s\n",cadUrl);
    #endif 
    Asignar_URL_stream_WIFI(cadUrl);    
    Leer_url_stream_WIFI(&leidos);
    #ifdef use_lib_wifi_debug
     Serial.printf("Leidos:%d\n",leidos); //Leemos 1024 bytes
    #endif 

    // Read in the registers    
    _zxCpu.i = gb_buffer_wifi[0];
    _zxCpu.registers.byte[Z80_L] = gb_buffer_wifi[1];
    _zxCpu.registers.byte[Z80_H] = gb_buffer_wifi[2];
    _zxCpu.registers.byte[Z80_E] = gb_buffer_wifi[3];
    _zxCpu.registers.byte[Z80_D] = gb_buffer_wifi[4];
    _zxCpu.registers.byte[Z80_C] = gb_buffer_wifi[5];
    _zxCpu.registers.byte[Z80_B] = gb_buffer_wifi[6];
    _zxCpu.registers.byte[Z80_F] = gb_buffer_wifi[7];
    _zxCpu.registers.byte[Z80_A] = gb_buffer_wifi[8];

    _zxCpu.alternates[Z80_HL] = _zxCpu.registers.word[Z80_HL];
    _zxCpu.alternates[Z80_DE] = _zxCpu.registers.word[Z80_DE];
    _zxCpu.alternates[Z80_BC] = _zxCpu.registers.word[Z80_BC];
    _zxCpu.alternates[Z80_AF] = _zxCpu.registers.word[Z80_AF];

    _zxCpu.registers.byte[Z80_L] = gb_buffer_wifi[9];
    _zxCpu.registers.byte[Z80_H] = gb_buffer_wifi[10];
    _zxCpu.registers.byte[Z80_E] = gb_buffer_wifi[11];
    _zxCpu.registers.byte[Z80_D] = gb_buffer_wifi[12];
    _zxCpu.registers.byte[Z80_C] = gb_buffer_wifi[13];
    _zxCpu.registers.byte[Z80_B] = gb_buffer_wifi[14];
    _zxCpu.registers.byte[Z80_IYL] = gb_buffer_wifi[15];
    _zxCpu.registers.byte[Z80_IYH] = gb_buffer_wifi[16];
    _zxCpu.registers.byte[Z80_IXL] = gb_buffer_wifi[17];
    _zxCpu.registers.byte[Z80_IXH] = gb_buffer_wifi[18];

    byte inter = gb_buffer_wifi[19];
    _zxCpu.iff2 = (inter & 0x04) ? 1 : 0;
    _zxCpu.r = gb_buffer_wifi[20];

    _zxCpu.registers.byte[Z80_F] = gb_buffer_wifi[21];
    _zxCpu.registers.byte[Z80_A] = gb_buffer_wifi[22];

    sp_l = gb_buffer_wifi[23];
    sp_h = gb_buffer_wifi[24];
    _zxCpu.registers.word[Z80_SP] = sp_l + sp_h * 0x100;

    _zxCpu.im = gb_buffer_wifi[25];
    byte bordercol = gb_buffer_wifi[26];

    borderTemp = bordercol;

    _zxCpu.iff1 = _zxCpu.iff2;

        uint16_t thestack = _zxCpu.registers.word[Z80_SP];
        uint16_t buf_p = 0x4000;
        contBuffer = 27;
        
        //He leido 1024 bytes. Lee resto 27
        int cont1024= 27;
        while (contBuffer< 50000)
        {
         writebyte(buf_p, gb_buffer_wifi[cont1024]);
         contBuffer++;
         buf_p++;
         cont1024++;
         if (cont1024 >= 1024)
         {
          Leer_url_stream_WIFI(&leidos);
          #ifdef use_lib_wifi_debug
           Serial.printf("Leidos:%d\n",leidos);
          #endif 
          cont1024= 0;
         }
        }

        retaddr = readword(thestack);
        #ifdef use_lib_log_serial
         Serial.printf("retaddr:%x\n", retaddr);
        #endif
        _zxCpu.registers.word[Z80_SP]++;
        _zxCpu.registers.word[Z80_SP]++;

    _zxCpu.pc = retaddr;

    KB_INT_START;
 }
 #endif 

#endif


//Lee de flash numero total de roms
void load_rom2flash(unsigned char is48k,unsigned char id)
{
 KB_INT_STOP;
 #ifdef use_lib_sound_ay8912 
  ResetSound();
 #endif 
 if (is48k == 1)
 {//Para 48K
  if (id < max_list_rom_48)
  {  
   #ifdef use_lib_core_jsanchezv
    #ifdef use_lib_rom0_inRAM_jsanchezv
     memcpy(rom0_inRAM_jsanchezv,gb_list_roms_48k_data[id],0x4000);
     rom0_jsanchezv= rom0_inRAM_jsanchezv;
    #else 
     rom0_jsanchezv= (uint8_t*)gb_list_roms_48k_data[id];
     ReloadLocalCacheROMram_jsanchezv();
    #endif
   #else      
    rom0= (uint8_t*)gb_list_roms_48k_data[id];    
    ReloadLocalCacheROMram(); //Recargo punteros rom ram
   #endif 
   //SetMode48K();
  }
 }
 else
 {//128k
  if (id < max_list_rom_128)
  {
   #ifdef use_lib_core_jsanchezv      
    rom0_jsanchezv= (uint8_t*)gb_list_roms_128k_data[id][0];
    rom1_jsanchezv= (uint8_t*)gb_list_roms_128k_data[id][1];
    rom2_jsanchezv= (uint8_t*)gb_list_roms_128k_data[id][2];
    rom3_jsanchezv= (uint8_t*)gb_list_roms_128k_data[id][3];   
    ReloadLocalCacheROMram_jsanchezv();
   #else
    rom0= (uint8_t*)gb_list_roms_128k_data[id][0];
    rom1= (uint8_t*)gb_list_roms_128k_data[id][1];
    rom2= (uint8_t*)gb_list_roms_128k_data[id][2];
    rom3= (uint8_t*)gb_list_roms_128k_data[id][3];
    ReloadLocalCacheROMram(); //Recargo punteros rom ram   
   #endif
   //SetMode128K();
  }
 }
 KB_INT_START;
}
