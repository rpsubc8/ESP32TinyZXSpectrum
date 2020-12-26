#include "gbConfig.h"
#include "def/hardware.h"
#include "startup.h"
#include <stdio.h>
#include <string.h>

#include "Emulator/Keyboard/PS2Kbd.h"
#include "Emulator/Memory.h"
#include "Emulator/clock.h"
#include "Emulator/z80Input.h"
#include "Emulator/z80main.h"
#include "gb_globals.h"

#define RAM_AVAILABLE 0xC000

Z80_STATE _zxCpu;

#ifdef use_lib_mouse_kempston
 int gb_z80_mouse_x;
 int gb_z80_mouse_y;
 int gb_z80_mouseBtn;
#endif 

int gb_screen_xIni=0;
int gb_screen_yIni=0;
unsigned char gbSoundSpeaker=0;
unsigned char gbSoundSpeakerBefore=0;
unsigned int gbContSPKChangeSamples=0;
//unsigned int gbContSPKSamples=0;
unsigned char gb_play_tape_on_now=0;
#ifdef use_lib_sound_ay8912
 int gb_ay8912_reg_select=0;
 int gb_ay8912_reg_value=0;
 int gb_ay8912_A_frec_fine=0;
 int gb_ay8912_B_frec_fine=0;
 int gb_ay8912_C_frec_fine=0;
 int gb_ay8912_A_frec_course=0;
 int gb_ay8912_B_frec_course=0;
 int gb_ay8912_C_frec_course=0;
 int gb_ay8912_noise_pitch=0;
 int gb_ay8912_A_frec=0;
 int gb_ay8912_B_frec=0;
 int gb_ay8912_C_frec=0;
 int gb_ay8912_A_vol=0;
 int gb_ay8912_B_vol=0;
 int gb_ay8912_C_vol=0;
 int gb_ay8912_mixer=0;
 unsigned char gb_silence_all_channels=0;
#endif 

#ifdef use_lib_vga_thread
 volatile unsigned char gbFrameSkipVideoMaxCont = gb_frame_crt_skip;
#else
 unsigned char gbDelayVideo=20;
#endif 

extern byte borderTemp;
extern byte z80ports_in[128];
//JJ extern byte z80ports_wiin[128];
extern byte tick;

CONTEXT _zxContext;
static uint16_t _attributeCount;
int _total;
int _next_total = 0;
static uint8_t zx_data = 0;
static uint32_t frames = 0;
static uint32_t _ticks = 0;
int cycles_per_step = CalcTStates();

static uint8_t * gb_local_cache_rom[4] = {
 rom0,rom1,rom2,rom3
};

static uint8_t * gb_local_cache_ram[8] =
{
 ram0,ram1,ram2,ram3,ram4,ram5,ram6,ram7
};

extern "C" {
//JJ uint8_t readbyte(uint16_t addr);
inline uint8_t fast_readbyte(uint16_t addr);
uint16_t readword(uint16_t addr);
void writebyte(uint16_t addr, uint8_t data);
void writeword(uint16_t addr, uint16_t data);
uint8_t input(uint8_t portLow, uint8_t portHigh);
void output(uint8_t portLow, uint8_t portHigh, uint8_t data);
}


void ReloadLocalCacheROMram()
{
 gb_local_cache_rom[0]=rom0;
 gb_local_cache_rom[1]=rom1;
 gb_local_cache_rom[2]=rom2;
 gb_local_cache_rom[3]=rom3;

 gb_local_cache_ram[0]=ram0;
 gb_local_cache_ram[1]=ram1;
 gb_local_cache_ram[2]=ram2;
 gb_local_cache_ram[3]=ram3;
 gb_local_cache_ram[4]=ram4;
 gb_local_cache_ram[5]=ram5;
 gb_local_cache_ram[6]=ram6;
 gb_local_cache_ram[7]=ram7;  
}

void zx_setup()
{
    _zxContext.fast_readbyte = fast_readbyte;
    _zxContext.readword = readword;
    _zxContext.writeword = writeword;
    _zxContext.writebyte = writebyte;
    _zxContext.input = input;
    _zxContext.output = output;

    zx_reset();
}

void zx_reset() {
    memset(z80ports_in, 0x1F, 128);
    borderTemp = 7;
    bank_latch = 0; gb_ptr_IdRomRam[3]=0;
    video_latch = 0;
    rom_latch = 0;
    paging_lock = 0;
    sp3_mode = 0;
    sp3_rom = 0;
    rom_in_use = 0;
    cycles_per_step = CalcTStates();

    Z80Reset(&_zxCpu);
}

//JJ int32_t 
void zx_loop() 
{
 //JJ int32_t result = -1;
 //JJ byte tmp_color = 0;
 //unsigned long ini_time,fin_time,aux_timeEmulate,aux_timeInterrupt;
 //ini_time=micros();

 _total = Z80Emulate(&_zxCpu, cycles_per_step, &_zxContext);
 //fin_time=micros();
 //aux_timeEmulate= fin_time-ini_time;

 //ini_time=micros();
 Z80Interrupt(&_zxCpu, 0xff, &_zxContext);
 //fin_time=micros();
 //aux_timeInterrupt= fin_time-ini_time;
    
 #ifdef use_lib_log_serial
  //Serial.printf("Total:%d Z80Emul:%d Z80Int:%d\n",_total,aux_timeEmulate,aux_timeInterrupt);
 #endif 

 //JJ return result;
}

unsigned char gb_ptr_IdRomRam[4]={
 0,5,2,0
}; //El ultimo es bank_latch

//Lectura rapida
extern "C" inline uint8_t fast_readbyte(uint16_t addr)
{

  unsigned char idRomRam = (addr>>14);
  if (idRomRam == 0)
   return (gb_local_cache_rom[rom_in_use][addr]);
  else
   return (gb_local_cache_ram[(gb_ptr_IdRomRam[idRomRam])][(addr & 0x3fff)]);

  /*unsigned char idRomRam = (addr>>14);
  switch(idRomRam)
  {
   case 0: return (gb_local_cache_rom[rom_in_use][addr]);  break;
   case 1: return ram5[(addr & 0x3fff)]; break;
   case 2: return ram2[(addr & 0x3fff)]; break;
   case 3: return (gb_local_cache_ram[bank_latch][(addr & 0x3fff)]); break;
  }*/

  /*switch (addr) 
  {
    case 0x0000 ... 0x3fff:
        return (gb_local_cache_rom[rom_in_use][addr]);
        break;
    case 0x4000 ... 0x7fff:
        return ram5[addr - 0x4000];
        break;
    case 0x8000 ... 0xbfff:
        return ram2[addr - 0x8000];
        break;
    case 0xc000 ... 0xffff:
        return (gb_local_cache_ram[bank_latch][(addr- 0xc000)]);        
        // Serial.printf("Address: %x Returned address %x  Bank: %x\n",addr,addr-0xc000,bank_latch);
        break;
  }    */
}

//extern "C" uint8_t readbyte(uint16_t addr) {
//    switch (addr) {
//    case 0x0000 ... 0x3fff:
//        switch (rom_in_use) {
//        case 0:
//            return rom0[addr];
//        case 1:
//            return rom1[addr];
//        case 2:
//            return rom2[addr];
//        case 3:
//            return rom3[addr];
//        }
//    case 0x4000 ... 0x7fff:
//        return ram5[addr - 0x4000];
//        break;
//    case 0x8000 ... 0xbfff:
//        return ram2[addr - 0x8000];
//        break;
//    case 0xc000 ... 0xffff:
//        switch (bank_latch) {
//        case 0:
//            return ram0[addr - 0xc000];
//            break;
//        case 1:
//            return ram1[addr - 0xc000];
//            break;
//        case 2:
//            return ram2[addr - 0xc000];
//            break;
//        case 3:
//            return ram3[addr - 0xc000];
//            break;
//        case 4:
//            return ram4[addr - 0xc000];
//            break;
//        case 5:
//            return ram5[addr - 0xc000];
//            break;
//        case 6:            
//            return ram6[addr - 0xc000]; //JJ            
//            break;
//        case 7:            
//            return ram7[addr - 0xc000]; //JJ            
//            break;
//        }
//        // Serial.printf("Address: %x Returned address %x  Bank: %x\n",addr,addr-0xc000,bank_latch);
//        break;
//    }
//}

extern "C" uint16_t readword(uint16_t addr) { return ((fast_readbyte(addr + 1) << 8) | fast_readbyte(addr)); }

extern "C" void writebyte(uint16_t addr, uint8_t data) {
   //Modo rapido
  unsigned char idRomRam = (addr>>14);
  if (idRomRam != 0)
   gb_local_cache_ram[(gb_ptr_IdRomRam[idRomRam])][(addr & 0x3fff)] = data;  

  /*unsigned char idRomRam = (addr>>14);
  switch(idRomRam)
  {
   case 0: return;
   case 1: ram5[(addr & 0x3fff)] = data; break;
   case 2: ram2[(addr & 0x3fff)] = data; break;   
   case 3: gb_local_cache_ram[bank_latch][(addr & 0x3fff)] = data; break;
  }
  return;*/

  /* switch (addr) 
   {
    case 0x0000 ... 0x3fff:
        return;
    case 0x4000 ... 0x7fff:
        ram5[addr - 0x4000] = data;
        break;
    case 0x8000 ... 0xbfff:
        ram2[addr - 0x8000] = data;
        break;
    case 0xc000 ... 0xffff:
        gb_local_cache_ram[bank_latch][(addr- 0xc000)] = data;
        break;
   }
   return;*/


    //  if (addr >= (uint16_t)0x4000 && addr <= (uint16_t)0x7FFF)
    //  {
    //      while (writeScreen) {
    //          delayMicroseconds(1);
    //      }
    // if (addr >= 0x8000)
    //   Serial.printf("Address: %x  Bank: %x\n",addr,bank_latch);

/*    switch (addr) {
    case 0x0000 ... 0x3fff:
        return;
    case 0x4000 ... 0x7fff:
        ram5[addr - 0x4000] = data;
        break;
    case 0x8000 ... 0xbfff:
        ram2[addr - 0x8000] = data;
        break;
    case 0xc000 ... 0xffff:
        switch (bank_latch) {
        case 0:
            ram0[addr - 0xc000] = data;
            break;
        case 1:
            ram1[addr - 0xc000] = data;
            break;
        case 2:
            ram2[addr - 0xc000] = data;
            break;
        case 3:
            ram3[addr - 0xc000] = data;
            break;
        case 4:
            ram4[addr - 0xc000] = data;
            break;
        case 5:
            ram5[addr - 0xc000] = data;
            break;
        case 6:            
            ram6[addr - 0xc000] = data; //JJ             
            break;
        case 7:            
            ram7[addr - 0xc000] = data; //JJ            
            break;
        }
        // Serial.println("plin");
        break;
    }
    return;*/
}

extern "C" void writeword(uint16_t addr, uint16_t data) {
    writebyte(addr, (uint8_t)data);
    writebyte(addr + 1, (uint8_t)(data >> 8));
}

extern "C" uint8_t input(uint8_t portLow, uint8_t portHigh) {
    int16_t kbdarrno = 0;
    // delay(2);
    // Serial.print ("IN ");

   #ifdef use_lib_mouse_kempston
    if (
        (portLow == 0xDF)
        && 
        ((portHigh == 0xFA)||(portHigh == 0xFB)||(portHigh == 0xFF))
        )
     {
      //printf("Kempston H:%x L:%x %d %d %d\n",portHigh,portLow,gb_z80_mouse_x,gb_z80_mouse_y,gb_z80_mouseBtn);
      switch (portHigh)
      {
       case 0xFB: return (gb_z80_mouse_x); break;
       case 0xFF: return (gb_z80_mouse_y); break;
       case 0xFA: return (gb_z80_mouseBtn); break;
       case 0xFE: return (0x80); break; //Detect mouse OK
       default: return 0xFF;
      }
     }
    #endif 

    if (portLow == 0xFE) {

        // EAR_PIN
//JJ         if (portHigh == 0xFE) {
//JJ             bitWrite(z80ports_in  [0], 6, digitalRead(EAR_PIN));
//JJ //JJ            bitWrite(z80ports_wiin[0], 6, digitalRead(EAR_PIN));
        //JJ }

        // Keyboard        
        uint8_t result = 0xFF;
        if (~(portHigh | 0xFE)&0xFF) result &= (z80ports_in[0] /*JJ& z80ports_wiin[0]*/); //JJ teclado
        if (~(portHigh | 0xFD)&0xFF) result &= (z80ports_in[1] /*JJ& z80ports_wiin[1]*/);
        if (~(portHigh | 0xFB)&0xFF) result &= (z80ports_in[2] /*JJ& z80ports_wiin[2]*/);
        if (~(portHigh | 0xF7)&0xFF) result &= (z80ports_in[3] /*JJ& z80ports_wiin[3]*/);
        if (~(portHigh | 0xEF)&0xFF) result &= (z80ports_in[4] /*JJ& z80ports_wiin[4]*/);
        if (~(portHigh | 0xDF)&0xFF) result &= (z80ports_in[5] /*JJ& z80ports_wiin[5]*/);
        if (~(portHigh | 0xBF)&0xFF) result &= (z80ports_in[6] /*JJ& z80ports_wiin[6]*/);
        if (~(portHigh | 0x7F)&0xFF) result &= (z80ports_in[7] /*JJ& z80ports_wiin[7]*/);
        return result;
    }
    // Kempston
    if (portLow == 0x1F) {
        return z80ports_in[31];
    }
    // Sound (AY-3-8912)

   #ifdef use_lib_sound_ay8912
    if (portLow == 0xFD)
    {
     switch (portHigh) 
     {
      case 0xFF:
      // Serial.println("Read AY register");
      //return _ay3_8912.getRegisterData();
      return (gb_ay8912_reg_value);
     }
    }
   #endif

    uint8_t data = zx_data;
    data |= (0xe0); /* Set bits 5-7 - as reset above */
    data &= ~0x40;
    // Serial.printf("Port %x%x  Data %x\n", portHigh,portLow,data);
    return data;
}

extern "C" void output(uint8_t portLow, uint8_t portHigh, uint8_t data) {
    uint8_t tmp_data = data;
    switch (portLow) {
    case 0xFE: {

        // delayMicroseconds(CONTENTION_TIME);

        // border color (no bright colors)
        bitWrite(borderTemp, 0, bitRead(data, 0));
        bitWrite(borderTemp, 1, bitRead(data, 1));
        bitWrite(borderTemp, 2, bitRead(data, 2));


        #ifndef use_lib_redirect_tapes_speaker                 
         #ifndef use_lib_resample_speaker
          #ifdef use_lib_ultrafast_speaker
           if (bitRead(data, 4))
            REG_WRITE(GPIO_OUT_W1TS_REG , BIT25); //High Set
           else 
            REG_WRITE(GPIO_OUT_W1TC_REG , BIT25); //High Set           
          #else
           digitalWrite(SPEAKER_PIN, bitRead(data, 4)); // speaker
          #endif 
         #endif
         #ifdef use_lib_resample_speaker
         gbSoundSpeaker = bitRead(data, 4);
         if (gbSoundSpeaker != gbSoundSpeakerBefore)
         {
          gbSoundSpeakerBefore = gbSoundSpeaker;
          gbContSPKChangeSamples++;
         }
         #endif         
        #endif
        //JJ digitalWrite(MIC_PIN, bitRead(data, 3)); // tape_out
        #ifdef use_lib_redirect_tapes_speaker         
         #ifndef use_lib_resample_speaker
          #ifdef use_lib_ultrafast_speaker           
           REG_WRITE((bitRead(data, 4) | bitRead(data, 3))?GPIO_OUT_W1TS_REG:GPIO_OUT_W1TC_REG , BIT25); //High Set                   
          #else
           digitalWrite(SPEAKER_PIN,(bitRead(data, 4) | bitRead(data, 3))); //redirection save tape          
          #endif 
         #endif
         #ifdef use_lib_resample_speaker
          gbSoundSpeaker = (bitRead(data, 4) | bitRead(data, 3));
          if (gbSoundSpeaker != gbSoundSpeakerBefore)
          {
           gbSoundSpeakerBefore = gbSoundSpeaker;
           gbContSPKChangeSamples++;
          }         
         #endif
        #endif 

        z80ports_in[0x20] = data;
    } break;

    case 0xFD: {
        // Sound (AY-3-8912)
        switch (portHigh)
        {
#ifdef use_lib_sound_ay8912
        case 0xFF:
            // Serial.printf("Select AY register %x %x %x\n",portHigh,portLow,data);
            //JJ _ay3_8912.selectRegister(data);
            //printf("AY8912 OUT 0xFFFD %d\n",data);
            gb_ay8912_reg_select= data;
            break;
        case 0xBF:
            // Serial.printf("Select AY register Data %x %x %x\n",portHigh,portLow,data);
            //JJ_ay3_8912.setRegisterData(data);
            //gbSoundSpeaker = (data>0)?1:0;
            //printf("AY8912 OUT 0xBFFD %d\n",data);
            gb_ay8912_reg_value= data;
            switch (gb_ay8912_reg_select)
            {
             case 0:
              gb_ay8912_A_frec_fine= gb_ay8912_reg_value;
              gb_ay8912_A_frec = ((gb_ay8912_A_frec_fine|((gb_ay8912_A_frec_course<<8)&0xF00)))+1;
              //gb_ay8912_A_frec = gb_frecuencia_ini+(26*gb_ay8912_A_frec_fine);
              //gb_ay8912_A_frec = gb_frecuencia_ini+(gb_ay8912_A_frec_fine<<5);
              //printf("Frec Fine A %d Hz %d\n",gb_ay8912_reg_value,gb_ay8912_A_frec);
              break;
             case 1: 
              //printf("Frec Course A %d\n",gb_ay8912_reg_value);
              gb_ay8912_A_frec_course= gb_ay8912_reg_value & 0x0F;
              gb_ay8912_A_frec = ((gb_ay8912_A_frec_fine|((gb_ay8912_A_frec_course<<8)&0xF00)))+1;
              //fine = n/2^o
              break;
             case 2: 
              gb_ay8912_B_frec_fine= gb_ay8912_reg_value;
              gb_ay8912_B_frec = ((gb_ay8912_B_frec_fine|((gb_ay8912_B_frec_course<<8)&0xF00)))+1;
              //gb_ay8912_B_frec = gb_frecuencia_ini+(26*gb_ay8912_B_frec_fine);
              //gb_ay8912_B_frec = gb_frecuencia_ini+(gb_ay8912_B_frec_fine<<5);
              //printf("Frec Fine B %d Hz %d\n",gb_ay8912_reg_value,gb_ay8912_B_frec);
              break;
             case 3: 
              //printf("Frec Course B %d\n",gb_ay8912_reg_value);
              gb_ay8912_B_frec_course= gb_ay8912_reg_value & 0x0F;
              gb_ay8912_B_frec = ((gb_ay8912_B_frec_fine|((gb_ay8912_B_frec_course<<8)&0xF00)))+1;
              break;
             case 4:
              gb_ay8912_C_frec_fine= gb_ay8912_reg_value;
              gb_ay8912_C_frec = ((gb_ay8912_C_frec_fine|((gb_ay8912_C_frec_course<<8)&0xF00)))+1;
              //gb_ay8912_C_frec = gb_frecuencia_ini+(26*gb_ay8912_C_frec_fine);
              //gb_ay8912_C_frec = gb_frecuencia_ini+(gb_ay8912_C_frec_fine<<5);
              //printf("Frec Fine C %d Hz %d\n",gb_ay8912_reg_value,gb_ay8912_C_frec);
              break;
             case 5: 
              //printf("Frec Course C %d\n",gb_ay8912_reg_value);
              gb_ay8912_C_frec_course= gb_ay8912_reg_value & 0x0F;
              gb_ay8912_C_frec = ((gb_ay8912_C_frec_fine|((gb_ay8912_C_frec_course<<8)&0xF00)))+1;
              break;
             case 6: gb_ay8912_noise_pitch; break;             
             case 7: 
              //printf("Mixer %d\n",gb_ay8912_reg_value);
              gb_ay8912_mixer= gb_ay8912_reg_value;
              break;
             case 8: 
              //printf("Vol A %d\n",gb_ay8912_reg_value);
              gb_ay8912_A_vol = gb_ay8912_reg_value;
              break;
             case 9: 
              //printf("Vol B %d\n",gb_ay8912_reg_value);
              gb_ay8912_B_vol = gb_ay8912_reg_value;
              break;
             case 10: 
              //printf("Vol C %d\n",gb_ay8912_reg_value);
              gb_ay8912_C_vol = gb_ay8912_reg_value;
              break;
              
        //INPUT "Note value ",n
        //INPUT "Octave ",o
        //LET f=INT(n/2^o)
        //LET c=INT(f/256)
        //PRINT "Fine = ";f;"  Course = ";c              
            }
            break;
#endif

        case 0x7F:
            if (!paging_lock) {
                paging_lock = bitRead(tmp_data, 5);
                rom_latch = bitRead(tmp_data, 4);
                video_latch = bitRead(tmp_data, 3);
                bank_latch = tmp_data & 0x7; gb_ptr_IdRomRam[3]= bank_latch;
                // rom_in_use=0;
                bitWrite(rom_in_use, 1, sp3_rom);
                bitWrite(rom_in_use, 0, rom_latch);
                // Serial.printf("7FFD data: %x ROM latch: %x Video Latch: %x bank latch: %x page lock:
                // %x\n",tmp_data,rom_latch,video_latch,bank_latch,paging_lock);
            }
            break;

        case 0x1F:
            sp3_mode = bitRead(data, 0);
            sp3_rom = bitRead(data, 2);
            bitWrite(rom_in_use, 1, sp3_rom);
            bitWrite(rom_in_use, 0, rom_latch);

            // Serial.printf("1FFD data: %x mode: %x rom bits: %x ROM chip: %x\n",data,sp3_mode,sp3_rom, rom_in_use);
            break;
        }
    } break;
        // default:
        //    zx_data = data;
        break;
    }
}


#ifdef use_lib_sound_ay8912
 //***************************
 void ResetSound()
 {
  gb_ay8912_reg_select=0;
  gb_ay8912_reg_value=0;
  gb_ay8912_A_frec_fine=0;
  gb_ay8912_B_frec_fine=0;
  gb_ay8912_C_frec_fine=0;
  gb_ay8912_A_frec_course=0;
  gb_ay8912_B_frec_course=0;
  gb_ay8912_C_frec_course=0;
  gb_ay8912_A_frec=gb_frecuencia_ini;
  gb_ay8912_B_frec=gb_frecuencia_ini;
  gb_ay8912_C_frec=gb_frecuencia_ini;
  gb_ay8912_noise_pitch=0;
  gb_ay8912_A_vol=0;
  gb_ay8912_B_vol=0;
  gb_ay8912_C_vol=0;
  gb_ay8912_mixer=0;
  #ifdef use_lib_resample_speaker
   gbContSPKChangeSamples = 0;  
  #endif 
 }
#endif
