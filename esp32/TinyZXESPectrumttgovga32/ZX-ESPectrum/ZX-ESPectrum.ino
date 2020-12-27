//Mod ackerman:
// - 128 KB NO PSRAM
// - Support AMX and Mouse Kempston (fabgl)
// - Tiny OSD
// - AY8912 dirty sound 3 channel (fabgl)
// - Speaker resample and mixer with AY8912 (fabgl)
// - SCR file load
// - File Tape BASIC load
// - File Tape SCREEN mem load
// - Out MIC tape sound to speaker
// - Remove sna save
// - Remove EAR and MIC tape hardware pinout
// - Remove WII pad
// - Mode thread or no thread dev experimental
// - Mode 320x200 no thread dev experimental
// - Mode low ram video dev experimental
// - Skip frame thread or delay video no thread
// - Screen offset X,Y
// - Mode bright compile
// - Mode 8 colour compile
// - Keyboard remap Shift+6, shift+7 and supr
// - Optimice video draw and CPU
// - Compatible Arduino IDE and PlatformIO
// - Customized parameters gbConfig.h
// - Arduino IDE 1.8.11
// - Library Arduino fabgl 0.9.0
// - Library Arduino bitluni 0.3.3 (opcional)
// - Arduino support espressif
// - Visual Studio 1.48.1 PLATFORMIO
//
// Basado en codigo de David Crespo ZX-ESPectrum-Wiimote
// -------------------------------------------------------------------
//  ESPectrum Emulator
//  Ramón Martínez & Jorge fuertes 2019
//  Though from ESP32 Spectrum Emulator (KOGEL esp32) Pete Todd 2017
// -------------------------------------------------------------------

#include "gbConfig.h"

#ifdef use_lib_sound_ay8912
 #include "fabgl.h" //Para fabgl
 #include "fabutils.h" //Para fabgl 
#endif 

#ifdef use_lib_mouse_kempston
 #include "fabgl.h" //Para fabgl
 #include "WiFiGeneric.h"; //Fix WiFiGeneric.h No such file or directory
 //#include "fabutils.h" //Para fabgl
 //#include <mouse.h>
#endif

#include "gbOptimice.h"
#include "dataFlash/gbrom.h"
#include "dataFlash/gbsna.h"
#include "Emulator/Keyboard/PS2Kbd.h"
#include "Emulator/Memory.h"
#include "Emulator/clock.h"
#include "Emulator/z80emu/z80emu.h"
#include "Emulator/z80main.h"
#include "Emulator/z80user.h"
#include <Arduino.h>
//Para ahorrar memoria
//JJ #include <esp_bt.h>

#include "MartianVGA.h"

#include "def/Font.h"
#include "def/hardware.h"
#include "driver/timer.h"
#include "soc/timer_group_struct.h"
#include "osd.h"
#include "gb_globals.h"

#ifdef use_lib_core_jsanchezv
 #include "jsanchezv_z80sim.h"
 //#include "gbPrograma.h"
#endif

extern int gb_screen_xIni;
extern int gb_screen_yIni;

#ifdef use_lib_sound_ay8912
 SoundGenerator soundGenerator;
 SineWaveformGenerator gb_sineArray[4];
 unsigned char gbVolMixer_before[4]={0,0,0,0}; 
 short int gbFrecMixer_before[4]={0,0,0,0};
 unsigned char gbVolMixer_now[4]={0,0,0,0};
 short int gbFrecMixer_now[4]={0,0,0,0};
#endif

#ifdef use_lib_resample_speaker
 unsigned long gbTimeIni=0;
 unsigned long gbTimeNow=0;
#endif 


#ifdef use_lib_mouse_kempston
 //fabgl::Mouse gb_mouse;
 fabgl::PS2Controller PS2Controller;
#endif 

#ifndef use_lib_vga_thread
 unsigned long gbTimeVideoIni=0;
 unsigned long gbTimeVideoNow=0;
#endif 

#ifdef use_lib_lookup_ram
 static unsigned char gb_lookup_calcY[200]={
#else
 //Son 6400, quedan en 200 se repiten 32 veces
 const unsigned char gb_lookup_calcY[200]={
#endif     
  0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x01,0x09,0x11,0x19,0x21,0x29,0x31,0x39,
  0x02,0x0A,0x12,0x1A,0x22,0x2A,0x32,0x3A,0x03,0x0B,0x13,0x1B,0x23,0x2B,0x33,0x3B,
  0x04,0x0C,0x14,0x1C,0x24,0x2C,0x34,0x3C,0x05,0x0D,0x15,0x1D,0x25,0x2D,0x35,0x3D,
  0x06,0x0E,0x16,0x1E,0x26,0x2E,0x36,0x3E,0x07,0x0F,0x17,0x1F,0x27,0x2F,0x37,0x3F,
  0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x41,0x49,0x51,0x59,0x61,0x69,0x71,0x79,
  0x42,0x4A,0x52,0x5A,0x62,0x6A,0x72,0x7A,0x43,0x4B,0x53,0x5B,0x63,0x6B,0x73,0x7B,
  0x44,0x4C,0x54,0x5C,0x64,0x6C,0x74,0x7C,0x45,0x4D,0x55,0x5D,0x65,0x6D,0x75,0x7D,
  0x46,0x4E,0x56,0x5E,0x66,0x6E,0x76,0x7E,0x47,0x4F,0x57,0x5F,0x67,0x6F,0x77,0x7F,
  0x80,0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0x81,0x89,0x91,0x99,0xA1,0xA9,0xB1,0xB9,
  0x82,0x8A,0x92,0x9A,0xA2,0xAA,0xB2,0xBA,0x83,0x8B,0x93,0x9B,0xA3,0xAB,0xB3,0xBB,
  0x84,0x8C,0x94,0x9C,0xA4,0xAC,0xB4,0xBC,0x85,0x8D,0x95,0x9D,0xA5,0xAD,0xB5,0xBD,
  0x86,0x8E,0x96,0x9E,0xA6,0xAE,0xB6,0xBE,0x87,0x8F,0x97,0x9F,0xA7,0xAF,0xB7,0xBF,
  0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,0xF8
 };



// EXTERN VARS
extern unsigned char gb_cfg_arch_is48K;
extern CONTEXT _zxContext;
extern Z80_STATE _zxCpu;
extern int _total;
extern int _next_total;
extern unsigned char gb_cache_zxcolor[8];
//static unsigned char gb_local_cache_zxcolor[8]=
//{//cache de colores optimizado
// BLACK, BLUE, RED, MAGENTA, GREEN, CYAN, YELLOW, WHITE
//};


//JJ void load_rom(String, String);
//JJ void load_ram(String);
//void load_ram2flash(unsigned char id);
//JJ void SetMode48K(void);
//JJ void SetMode128K(void);

volatile unsigned char gb_sdl_blit=0;
static unsigned long gb_currentTime=0;
#ifdef use_lib_sound_ay8912
 static unsigned long gb_sdl_time_sound_before=0;
#endif 
static unsigned long gb_keyboardTime;
static unsigned long gb_time_ini_espera;
unsigned char gb_run_emulacion = 1; //Ejecuta la emulacion
unsigned char gb_current_ms_poll_sound = gb_ms_sound;
unsigned char gb_current_ms_poll_keyboard = gb_ms_keyboard;
//unsigned char gb_current_frame_crt_skip= gb_frame_crt_skip; //No salta frames
unsigned char gb_current_delay_emulate_ms= gb_delay_emulate_ms;
unsigned char gb_current_delay_emulate_div_microsec= gb_delay_emulate_div_microsec;


volatile byte keymap[256];
volatile byte oldKeymap[256];

volatile unsigned char gbFrameSkipVideoCurrentCont=0;

// EXTERN METHODS
//void setup_cpuspeed();


static uint8_t * gb_local_cache_rom[4] = {
 rom0,rom1,rom2,rom3
};

static uint8_t * gb_local_cache_ram[8] =
{
 ram0,ram1,ram2,ram3,ram4,ram5,ram6,ram7
};

/*uint8_t local_fast_readbyte(uint16_t addr)
{
  switch (addr) 
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
  }    
}*/

void swap_flash(word *a, word *b);

// GLOBALS

// keyboard ports read from PS2 keyboard
volatile byte z80ports_in[128];

// keyboard ports read from Wiimote
//JJ volatile byte z80ports_wiin[128];

volatile byte borderTemp = 7;
volatile byte flashing = 0;
volatile boolean xULAStop = false;
volatile boolean xULAStopped = false;
volatile byte tick;
//const int SAMPLING_RATE = 44100;
//const int BUFFER_SIZE = 2000;

int halfsec, sp_int_ctr, evenframe, updateframe;

#ifdef use_lib_vga_thread
 QueueHandle_t vidQueue;
 TaskHandle_t videoTaskHandle;
 volatile bool videoTaskIsRunning = false;
 uint16_t *param;
#endif

// SETUP *************************************
#ifdef use_lib_vga8colors
 #ifdef use_lib_vga_low_memory
  VGA3BitI vga;
 #else
  VGA3Bit vga;
 #endif
#else
 #ifdef use_lib_vga64colors
  #ifdef use_lib_vga_low_memory
   VGA6BitI vga;
  #else
   VGA6Bit vga;
  #endif  
 #endif
#endif

//JJ #ifdef COLOR_14B
//JJ VGA14Bit vga;
//JJ #endif

#ifdef use_lib_sound_ay8912
 inline void sound_cycleFabgl(void);
 inline void jj_mixpsg(void);
#endif

#ifdef use_lib_mouse_kempston
 void PollMouse(void);
#endif

#ifdef use_lib_remap_keyboardpc 
 void do_keyboard_remap_pc(void);
#endif 

#ifdef use_lib_core_jsanchezv
 void loop_jsanchezv(void);
#endif

#ifdef use_lib_core_jsanchezv
 unsigned char * rom0_jsanchezv;
 unsigned char * ram5_jsanchezv;
 unsigned char * ram2_jsanchezv;
 static unsigned char * gb_ram_z80Ram_jsanchezv;
 static unsigned char * gb_ram_z80Ports_jsanchezv;

 Z80sim::Z80sim(void) : cpu(this)
 {

 }

 Z80sim::~Z80sim() {}

 void Z80sim::AssignPtrRamPort(unsigned char *ptrRam,unsigned char *ptrPort)
 {
  z80Ram = ptrRam;
  z80Ports = ptrPort;
 }

uint8_t Z80sim::fetchOpcode(uint16_t address) {
 // 3 clocks to fetch opcode from RAM and 1 execution clock
 tstates += 4;
 unsigned char idRomRam = (address>>14);
 unsigned char aux_fetchOpcode=0; 
 if (idRomRam == 0)
  aux_fetchOpcode= rom0_jsanchezv[address];
 else
  aux_fetchOpcode = z80Ram[address]; 
  
 //printf ("fetchOpcode:%02x add:%d",aux_fetchOpcode,address);
 return aux_fetchOpcode;

//JJ  #ifdef WITH_BREAKPOINT_SUPPORT
//JJ   return z80Ram[address];
//JJ  #else
//JJ   uint8_t opcode = z80Ram[address];
//JJ   return (address != 0x0005 ? opcode : breakpoint(address, opcode));
//JJ  #endif
}

uint8_t Z80sim::peek8(uint16_t address) {
 // 3 clocks for read byte from RAM
 tstates += 3;
 //JJ return z80Ram[address];    
 //printf ("peek8 add:%d\n",address);
 unsigned char idRomRam = (address>>14);
 //if (idRomRam == 0)
 // return (rom0_jsanchezv[address]);
 //else
 // return (z80Ram[address]);
  
 switch(idRomRam)
 {
  case 0: return (rom0_jsanchezv[address]);  break;
  case 1: 
   //printf ("rram5 add:%d\n",address);
   return ram5_jsanchezv[(address & 0x3fff)]; 
   break;
  case 2:
   //printf ("rram2 add:%d\n",address); 
   return ram2_jsanchezv[(address & 0x3fff)]; 
   break;
  case 3: 
   //printf ("rram:%d\n",address); 
   return (z80Ram[address]);
   break;
 }
}

void Z80sim::poke8(uint16_t address, uint8_t value) {
 // 3 clocks for write byte to RAM
 tstates += 3;
 //JJ z80Ram[address] = value;
 //printf ("poke8 add:%d\n",address);
 unsigned char idRomRam = (address>>14);
 //if (idRomRam != 0)
 // z80Ram[address] = value;
  
 switch(idRomRam)
 {
  //case 0: return (rom0_jsanchezv[address]);  break;
  case 1: 
   //if (value!=0)
   // printf ("wram5 add:%d val:%d add:%d\n",address,value,(address & 0x3fff)); 
   ram5_jsanchezv[(address & 0x3fff)] = value;
   break;
  case 2: 
   //printf ("wram2 add:%d\n",address);
   ram2_jsanchezv[(address & 0x3fff)] = value; 
   break;
  case 3:
   //printf ("wram:%d\n",address);
   z80Ram[address] = value; 
   break;
 }  
}

uint16_t Z80sim::peek16(uint16_t address) {
 //printf ("peek16 add:%d\n",address);
    // Order matters, first read lsb, then read msb, don't "optimize"
    uint8_t lsb = peek8(address);
    uint8_t msb = peek8(address + 1);
    return (msb << 8) | lsb;
}

void Z80sim::poke16(uint16_t address, RegisterPair word) {
 //printf ("poke16 add:%d\n",address);     
    // Order matters, first write lsb, then write msb, don't "optimize"
    poke8(address, word.byte8.lo);
    poke8(address + 1, word.byte8.hi);
}

uint8_t Z80sim::inPort(uint16_t port) {
    // 4 clocks for read byte from bus
 //printf ("inPort port:%d\n",port);    
    tstates += 3;
    return z80Ports[port];
}

void Z80sim::outPort(uint16_t port, uint8_t value) {
 //printf ("outPort outPort:%d\n",port);         
    // 4 clocks for write byte to bus
    tstates += 4;
    z80Ports[port] = value;
}

void Z80sim::addressOnBus(uint16_t address, int32_t tstates) {
 //printf ("addressOnBus:%d\n",address);
    // Additional clocks to be added on some instructions
    this->tstates += tstates;
}

void Z80sim::interruptHandlingTime(int32_t tstates) {
// printf ("interruptHandlingTime\n");
    this->tstates += tstates;
}

bool Z80sim::isActiveINT(void) {
// printf ("isActiveINT\n");     
	// Put here the needed logic to trigger an INT
    return false;
}

#ifdef WITH_EXEC_DONE
void Z80sim::execDone(void) {}
#endif

uint8_t Z80sim::breakpoint(uint16_t address, uint8_t opcode) {
    // Emulate CP/M Syscall at address 5

#ifdef WITH_BREAKPOINT_SUPPORT
    if (address != 0x0005)
         return opcode;
#endif

    //printf ("cpu.getRegC %d opcode:%d\n",cpu.getRegC(),opcode);
    switch (cpu.getRegC()) {
        case 0: // BDOS 0 System Reset
        {
            //JJcout << "Z80 reset after " << tstates << " t-states" << endl;
            //printf ("Z80 reset after %d t-states\n",tstates);
            finish = true;
            break;
        }
        case 2: // BDOS 2 console char output
        {
            //JJ cout << (char) cpu.getRegE();
            //printf("BDOS 2 %c",(char) cpu.getRegE());
            break;
        }
        case 9: // BDOS 9 console string output (string terminated by "$")
        {
         char jj_cad[128]="";
         int jj_cont_cad=0;
         memset(jj_cad,0,50);
         jj_cad[0]='\0';
         
            uint16_t strAddr = cpu.getRegDE();
            while (z80Ram[strAddr] != '$')
            {
             //cout << (char) z80Ram[strAddr++];
             jj_cad[jj_cont_cad]=(char)z80Ram[strAddr];            
             strAddr++;
             if (jj_cont_cad>=50)
              break;
             if (jj_cont_cad<50)
              jj_cont_cad++;             
            }
            jj_cad[jj_cont_cad]='\0';
            //printf("BDOS 9 len:%d dat:%s\n",jj_cont_cad,jj_cad);
            //cout.flush();
            break;
        }
        default:
        {
            //cout << "BDOS Call " << cpu.getRegC() << endl;
            //printf("BDOS Call %c\n",cpu.getRegC());
            finish = true;
            //printf("finish\n");
            //cout << finish << endl;
        }
    }
    // opcode would be modified before the decodeOpcode method
    return opcode;
}

void Z80sim::runTestJJ_poll(void)
{
 if (!finish) {
  cpu.execute();
  //printf("1\n");
  //printf("gb_cont: %d\n",gb_cont);
  //gb_cont++;
 }
}

void Z80sim::runTestJJ(void)
{
 memset(z80Ram,0,sizeof z80Ram);
 memset(z80Ports,0,sizeof z80Ports);
// memcpy(&z80Ram[0x100],gb_programa,8590);
 
    cpu.reset();
    finish = false;
    
  //cpu.setRegPC(0x3B5);
//  memcpy(z80Ram,rom0_jsanchezv,16384);
 // for (int i=0; i<50;i++)
 //  printf ("%02x",rom0_jsanchezv[i]);
 // printf("\n"); 

/*    z80Ram[0] = (uint8_t) 0xC3;
    z80Ram[1] = 0x00;
    z80Ram[2] = 0x01; // JP 0x100 CP/M TPA
    z80Ram[5] = (uint8_t) 0xC9; // Return from BDOS call

    //cpu.setBreakpoint(0x0005, true);
    #ifdef WITH_BREAKPOINT_SUPPORT
     cpu.setBreakpoint(true);
     printf("cpu.setBreakpoint(true)\n");
    #endif    */
    //while (!finish) {
    //    cpu.execute();
    //} 
}

/*void Z80sim::runTest(std::ifstream* f) {
    streampos size;
    if (!f->is_open()) {
        cout << "file NOT OPEN" << endl;
        return;
    } else cout << "file open" << endl;

    size = f->tellg();
    cout << "Test size: " << size << endl;
    f->seekg(0, ios::beg);
    f->read((char *) &z80Ram[0x100], size);
    f->close();

#ifdef WITH_BREAKPOINT_SUPPORT
    cpu.setBreakpoint(true);
#endif

    cpu.reset();
    finish = false;

    z80Ram[0] = (uint8_t) 0xC3;
    z80Ram[1] = 0x00;
    z80Ram[2] = 0x01; // JP 0x100 CP/M TPA
    z80Ram[5] = (uint8_t) 0xC9; // Return from BDOS call

    while (!finish) {
        cpu.execute();
    }
}*/

Z80sim sim = Z80sim();
#endif


#ifdef use_lib_core_jsanchezv
 //Emulacion de jsanchezv()
 void loop_jsanchezv()
 {
  //printf("loop_jsanchezv\n");
  sim.runTestJJ_poll();
 }
#endif

void setup()
{
 //DO NOT turn off peripherals to recover some memory
 //esp_bt_controller_deinit(); //Reduzco consumo RAM
 //esp_bt_controller_mem_release(ESP_BT_MODE_BTDM); //Reduzco consumo RAM
 #ifdef use_lib_log_serial
  Serial.begin(115200);         
  Serial.printf("HEAP BEGIN %d\n", ESP.getFreeHeap());
  //JJSerial.printf("PSRAM size: %d\n", ESP.getPsramSize());
 #endif

 #ifdef use_lib_lookup_ram
  gb_lookup_calcY[0]= 0; //force ram compiler,not const progmem
 #endif

 //SetMode48K();
 rom0 = (uint8_t *)gb_rom_0_sinclair_48k;    
 rom1 = (uint8_t *)gb_rom_0_sinclair_48k;
 rom2 = (uint8_t *)gb_rom_0_sinclair_48k;
 rom3 = (uint8_t *)gb_rom_0_sinclair_48k;
 
 #ifdef use_lib_core_rampa069
  ram0 = (byte *)malloc(16384);
  ram1 = (byte *)malloc(16384);
  ram2 = (byte *)malloc(16384);
  ram3 = (byte *)malloc(16384);
  ram4 = (byte *)malloc(16384);
  ram5 = (byte *)malloc(16384);
  ram6 = (byte *)malloc(16384);    
  ram7 = (byte *)malloc(16384);
 #else
  #ifdef use_lib_core_jsanchezv    
   gb_ram_z80Ram_jsanchezv = (unsigned char *)malloc(0x10000);
   gb_ram_z80Ports_jsanchezv = (unsigned char *)malloc(0x10000);
   sim.AssignPtrRamPort(gb_ram_z80Ram_jsanchezv,gb_ram_z80Ports_jsanchezv);
  #endif
 #endif

 //SDL_Generate_lookup_calcY();
 #ifdef use_lib_log_serial
  Serial.printf("HEAP AFTER RAM %d\n", ESP.getFreeHeap());
 #endif 

 #ifdef use_lib_vga8colors
  #ifndef use_lib_vga320x200
   #ifdef use_lib_vga320x240
    vga.init(vga.MODE320x240, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);        
   #else
    vga.init(vga.MODE360x200, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);    
   #endif 
  #else   
   vga.init(vga.MODE320x200, RED_PIN_3B, GRE_PIN_3B, BLU_PIN_3B, HSYNC_PIN, VSYNC_PIN);       
  #endif     
 #endif

#ifdef COLOR_6B
   const int redPins[] = {RED_PINS_6B};
   const int grePins[] = {GRE_PINS_6B};
   const int bluPins[] = {BLU_PINS_6B};
   #ifndef use_lib_vga320x200
    #ifdef use_lib_vga320x240
     vga.init(vga.MODE320x240, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
    #else   
     vga.init(vga.MODE360x200, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
    #endif
   #else
    vga.init(vga.MODE320x200, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
   #endif 
#endif

#ifdef COLOR_14B
    const int redPins[] = {RED_PINS_14B};
    const int grePins[] = {GRE_PINS_14B};
    const int bluPins[] = {BLU_PINS_14B};
    vga.init(vga.MODE360x200, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
#endif
    
    #ifdef use_lib_log_serial
     Serial.printf("HEAP after vga  %d \n", ESP.getFreeHeap());
    #endif
    vga.setFont(Font6x8);
    vga.clear(BLACK);    
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

    #ifndef use_lib_resample_speaker
     pinMode(SPEAKER_PIN, OUTPUT);
     #ifdef use_lib_ultrafast_speaker
      REG_WRITE(GPIO_OUT_W1TC_REG , BIT25); //LOW clear
     #else 
      digitalWrite(SPEAKER_PIN, LOW);
     #endif 
    #endif 
    //JJ pinMode(EAR_PIN, INPUT);
    //JJ pinMode(MIC_PIN, OUTPUT);
    
    //JJ digitalWrite(MIC_PIN, LOW);

    #ifdef use_lib_mouse_kempston
     //gb_mouse.begin(GPIO_NUM_26, GPIO_NUM_27);
     //PS2Controller.begin(GPIO_NUM_26, GPIO_NUM_27); // clk, dat
     //gb_mouse.begin(0); 
     PS2Controller.begin();
    #endif 
    
    kb_begin();    

    //JJ Serial.printf("%s bank %u: %ub\n", MSG_FREE_HEAP_AFTER, 0, ESP.getFreeHeap());
    //JJ Serial.printf("CALC TSTATES/PERIOD %u\n", CalcTStates());

    // START Z80
    //JJ Serial.println(MSG_Z80_RESET);
    ReloadLocalCacheROMram(); //Recargo punteros RAM ROM
    zx_setup();

    // make sure keyboard ports are FF
    memset((void *)z80ports_in,0x1f,32);//Optimice resize code
    
    #ifdef use_lib_vga_thread
     #ifdef use_lib_core_rampa069
      vidQueue = xQueueCreate(1, sizeof(uint16_t *));
      xTaskCreatePinnedToCore(&videoTask, "videoTask", 1024 * 4, NULL, 5, &videoTaskHandle, 0);
     #endif 
    #endif

    #ifdef use_lib_sound_ay8912
     #ifndef use_lib_resample_speaker
      for (unsigned char i=0;i<3;i++)
      {
       soundGenerator.attach(&gb_sineArray[i]);
       gb_sineArray[i].enable(true);
       gb_sineArray[i].setFrequency(0);
      }
      soundGenerator.play(true);
     #endif 
    #endif

    #ifdef use_lib_resample_speaker
     for (unsigned char i=0;i<4;i++)
     {
      soundGenerator.attach(&gb_sineArray[i]);
      gb_sineArray[i].enable(true);
      gb_sineArray[i].setFrequency(0);
     }
     soundGenerator.play(true);
    #endif 
    
    #ifdef use_lib_log_serial
     Serial.printf("End of setup RAM %d\n",ESP.getFreeHeap());
     //Serial.printf("mask %d bits %d\n",vga.RGBAXMask,vga.SBits);  
    #endif 
    gb_keyboardTime = millis();
    #ifdef use_lib_sound_ay8912
     gb_sdl_time_sound_before = gb_keyboardTime;
    #endif
        
}


#ifdef use_lib_sound_ay8912
 void SilenceAllChannels()
 {
  #ifdef use_lib_resample_speaker
   for (unsigned char i=0;i<4;i++)
  #endif
  #ifndef use_lib_resample_speaker
   for (unsigned char i=0;i<3;i++)
  #endif  
  {
   gb_sineArray[i].setFrequency(0);
   gb_sineArray[i].setVolume(0);
   gbVolMixer_before[i]= gbVolMixer_now[i]= 0;
   gbFrecMixer_before[i]= gbFrecMixer_now[i]= 0;
  }
  gb_ay8912_A_frec= gb_ay8912_B_frec= gb_ay8912_C_frec= 0;
  gb_ay8912_A_vol= gb_ay8912_B_vol= gb_ay8912_C_vol= 0;
 }


 inline void sound_cycleFabgl()
 {  
  //AY8912
  for (unsigned char i=0;i<3;i++)
  {
   if (gbVolMixer_now[i] != gbVolMixer_before[i])
   {
    gb_sineArray[i].setVolume((gbVolMixer_now[i]<<2));
    gbVolMixer_before[i] = gbVolMixer_now[i];
   }
   if (gbFrecMixer_now[i] != gbFrecMixer_before[i])
   {
    gb_sineArray[i].setFrequency(gbFrecMixer_now[i]);
    gbFrecMixer_before[i] = gbFrecMixer_now[i];
   }
  }
 }
 
 inline void jj_mixpsg() 
 {//AY8912  
  int auxFrec;
  gbVolMixer_now[0]= gb_ay8912_A_vol;
  gbVolMixer_now[1]= gb_ay8912_B_vol;
  gbVolMixer_now[2]= gb_ay8912_C_vol;
  if (gbVolMixer_now[0] == 0) gbFrecMixer_now[0] = 0;
  else{
   gbVolMixer_now[0]=15;
   auxFrec = gb_ay8912_A_frec;
   auxFrec = (auxFrec>0)?(62500/auxFrec):0;
   if (auxFrec>15000) auxFrec=15000;
   gbFrecMixer_now[0] = auxFrec;
  }
  if (gbVolMixer_now[1] == 0) gbFrecMixer_now[1] = 0;
  else{
   gbVolMixer_now[1]=15;
   auxFrec = gb_ay8912_B_frec;
   auxFrec = (auxFrec>0)?(62500/auxFrec):0;
   if (auxFrec>15000) auxFrec=15000;   
   gbFrecMixer_now[1] = auxFrec;
  }
  if (gbVolMixer_now[2] == 0) gbFrecMixer_now[2] = 0;
  else{
   gbVolMixer_now[2]=15;
   auxFrec = gb_ay8912_C_frec;
   auxFrec = (auxFrec>0)?(62500/auxFrec):0;
   if (auxFrec>15000) auxFrec=15000;   
   gbFrecMixer_now[2] = auxFrec;
  }
  
 /*
  //AY8912  
  int auxFrec = gb_ay8912_A_frec;
  int auxVol = gb_ay8912_A_vol;

  if (auxVol != gbVolMixer_before[0]){
   gb_sineArray[0].setVolume((auxVol<<2));
   gbVolMixer_before[0] = auxVol;
  }
  if (auxFrec != gbFrecMixer_before[0]){
   gb_sineArray[0].setFrequency(auxFrec);
   gbFrecMixer_before[0] = auxFrec;
  }
  
  auxFrec = gb_ay8912_B_frec + (gb_ay8912_B_frec_course<<2); //*4
  auxVol = gb_ay8912_B_vol<<1; //*2
  if (auxVol != gbVolMixer_before[1]){
   gb_sineArray[1].setVolume(auxVol);
   gbVolMixer_before[1] = auxVol;
  }
  if (auxFrec != gbFrecMixer_before[1]){
   gb_sineArray[1].setFrequency(auxFrec);
   gbFrecMixer_before[1] = auxFrec;
  }  

  auxFrec = gb_ay8912_C_frec + (gb_ay8912_C_frec_course<<2); //*4
  auxVol = gb_ay8912_C_vol<<1; //*2
  if (auxVol != gbVolMixer_before[2]){
   gb_sineArray[2].setVolume(auxVol);
   gbVolMixer_before[2] = auxVol;
  }
  if (auxFrec != gbFrecMixer_before[2]){
   gb_sineArray[2].setFrequency(auxFrec);
   gbFrecMixer_before[2] = auxFrec;
  }
  */

  #ifdef use_lib_resample_speaker
   gbTimeNow = millis();
   if ((gbTimeNow-gbTimeIni)>=25)
   {
    gbTimeIni = gbTimeNow;
    auxFrec = gbContSPKChangeSamples<<3;
    auxVol = (gbContSPKChangeSamples != 0)? 32: 0;    
    if (auxVol != gbVolMixer_before[3]){
     gb_sineArray[3].setVolume(auxVol);
     gbVolMixer_before[3] = auxVol;
    }    
    if (auxFrec != gbFrecMixer_before[3])
    {
     gb_sineArray[3].setFrequency(auxFrec);
     gbFrecMixer_before[3] = auxFrec;
    }     
    gbContSPKChangeSamples = 0;
   }
  #endif
   
  //gb_sineArray[1].setVolume((gb_ay8912_B_vol<<1));
  //gb_sineArray[1].setFrequency((gb_ay8912_B_frec + ((gb_ay8912_B_frec_course<<2))));
  //gb_sineArray[2].setVolume((gb_ay8912_C_vol<<1));
  //gb_sineArray[2].setFrequency((gb_ay8912_C_frec + ((gb_ay8912_C_frec_course<<2))));
  

  
  /*gb_vol_array_cur[0] = (gb_ay8912_A_vol<<1);
  //if ((gb_ay8912_mixer & 0x01)==0x00)
  {      
   //if (gb_ay8912_A_vol>0)
    gb_frec_array_cur[0]= gb_ay8912_A_frec + ((gb_ay8912_A_frec_course<<2));   
  }
  //else  
  // gb_vol_array_cur[0] = 0;

  gb_vol_array_cur[1] = (gb_ay8912_B_vol<<1);
  //if ((gb_ay8912_mixer & 0x02)==0x00)
  {   
   //if (gb_ay8912_B_vol>0)
    gb_frec_array_cur[1]= gb_ay8912_B_frec + ((gb_ay8912_B_frec_course<<2));   
  }
  //else
  // gb_vol_array_cur[1] = 0;
  
    
  gb_vol_array_cur[2] = (gb_ay8912_C_vol<<1); 
  //if ((gb_ay8912_mixer & 0x04)==0x00)
  {  
   //if (gb_ay8912_B_vol>0)   
    gb_frec_array_cur[2]= gb_ay8912_C_frec  + ((gb_ay8912_C_frec_course<<2));   
  }
  //else
  // gb_vol_array_cur[2] = 0;

  //if (gb_silence==1)  
  // gb_frec_ch01_cur= gb_frec_ch02_cur= gb_frec_ch03_cur = 0;  

 // if ((gb_ay8912_mixer & 0x08) == 0x00)
 //  gb_frec_array_cur[0] = gb_frec_array_cur[0] + rand() % 20;
 // if ((gb_ay8912_mixer & 0x10) == 0x00)   
 //  gb_frec_array_cur[1] = gb_frec_array_cur[1] + rand() % 20;
 // if ((gb_ay8912_mixer & 0x20) == 0x00)
 //  gb_frec_array_cur[2] = gb_frec_array_cur[2] + rand() % 20;

  for (unsigned char i=0;i<3;i++)
  {
   gb_sineArray[i].setFrequency(gb_frec_array_cur[i]);   
   gb_sineArray[i].setVolume(gb_vol_array_cur[i]);
  }   
  */
 }
#endif 

#ifdef use_lib_mouse_kempston
void PollMouse(void)
{
 auto mouse = PS2Controller.mouse();
 if (mouse->deltaAvailable())
 {
  MouseDelta mouseDelta;
  mouse->getNextDelta(&mouseDelta);
  gb_z80_mouseBtn = 0x00;
  if (mouseDelta.buttons.right)
   gb_z80_mouseBtn = gb_z80_mouseBtn | 0x01;
  if (mouseDelta.buttons.left)
  {
   gb_z80_mouseBtn = gb_z80_mouseBtn | 0x02;
   keymap[KEY_ALT_GR] = 0;
  }
  else
   keymap[KEY_ALT_GR] = 1;
  if (mouseDelta.buttons.middle)   
   gb_z80_mouseBtn = gb_z80_mouseBtn | 0x04;
  gb_z80_mouseBtn = (~gb_z80_mouseBtn)|0x08; //KMouse protocol always 1 bit3

  gb_z80_mouse_x = gb_z80_mouse_x + mouseDelta.deltaX;
  gb_z80_mouse_y = gb_z80_mouse_y + mouseDelta.deltaY;

  keymap[KEY_CURSOR_RIGHT] = (mouseDelta.deltaX > 0)?0:1;       
  keymap[KEY_CURSOR_LEFT] = (mouseDelta.deltaX < 0)?0:1;
  keymap[KEY_CURSOR_DOWN] = (mouseDelta.deltaY < 0)?0:1;
  keymap[KEY_CURSOR_UP] = (mouseDelta.deltaY > 0 )?0:1; 
   
  
  //Serial.printf("deltaX = %d  deltaY = %d  deltaZ = %d  leftBtn = %d  midBtn = %d  rightBtn = %d\n",
  //              mouseDelta.deltaX, mouseDelta.deltaY, mouseDelta.deltaZ,
  //              mouseDelta.buttons.left, mouseDelta.buttons.middle, mouseDelta.buttons.right);
 } 
}
#endif

// VIDEO core 0 *************************************

/*
//Medio optimizado
void videoTask(void *unused)
{
    unsigned long time_prev;
    unsigned int ff, i, byte_offset;
    unsigned char color_attrib, pixel_map, flash, bright;
    unsigned int zx_vidcalc, calc_y;

    word zx_fore_color, zx_back_color, tmp_color;
    // byte active_latch;

    videoTaskIsRunning = true;
    uint16_t *param;
    //int auxY;
    //unsigned char auxColor;

    while (1) 
    {        
        xQueuePeek(vidQueue, &param, portMAX_DELAY);
        if ((int)param == 1)
            break;

        time_prev = micros();   

        for (unsigned int vga_lin = 0; vga_lin < 200; vga_lin++) {
          //for (unsigned int vga_lin = 0; vga_lin < 150; vga_lin++) {
            // tick = 0;
            if (vga_lin < 3 || vga_lin > 194) {
            //if (vga_lin < 3 || vga_lin > 144) {
                //auxY= vga_lin^2;
                //auxColor= (gb_cache_zxcolor[borderTemp] & 0x3f) | 0xc0;
                for (int bor = 32; bor < 328; bor++)
                {
                //for (int bor = 32; bor < 320; bor++) //360-328=32
                //for (int bor = 32; bor < 188; bor++) //360-328=32
                    //JJvga.dotFast(bor, vga_lin, zxcolor(borderTemp, 0));
                 vga.dotFast(bor, vga_lin, gb_cache_zxcolor[borderTemp]);
                 //vga.backBuffer[auxY][bor] = auxColor;
                }
            } else {

                for (int bor = 32; bor < 52; bor++)
                {
                    //JJvga.dotFast(bor, vga_lin, zxcolor(borderTemp, 0));
                    //JJvga.dotFast(bor + 276, vga_lin, zxcolor(borderTemp, 0));
                    vga.dotFast(bor, vga_lin, gb_cache_zxcolor[borderTemp]);
                    vga.dotFast(bor + 276, vga_lin, gb_cache_zxcolor[borderTemp]);

                    //vga.dotFast(bor + 268, vga_lin, zxcolor(borderTemp, 0));
                    //vga.dotFast(bor + 140, vga_lin, zxcolor(borderTemp, 0));
                }

                for (ff = 0; ff < 32; ff++) // foreach byte in line
                {
                    byte_offset = (vga_lin - 3) * 32 + ff;
                    calc_y = calcY(byte_offset);
                    if (!video_latch)
                    {
                       color_attrib = readbyte(0x5800 + (calc_y / 8) * 32 + ff); // get 1 of 768 attrib values
                       pixel_map = readbyte(byte_offset + 0x4000);
                     } else
                      {
                       color_attrib = ram7[0x1800 + (calc_y / 8) * 32 + ff]; // get 1 of 768 attrib values                        
                       pixel_map = ram7[byte_offset];//JJ                        
                      }

                    for (i = 0; i < 8; i++) // foreach pixel within a byte
                    {

                        zx_vidcalc = ff * 8 + i;
                        byte bitpos = (0x80 >> i);
                        bright = (color_attrib & 0B01000000) >> 6;
                        flash = (color_attrib & 0B10000000) >> 7;
                        //JJ zx_fore_color = zxcolor((color_attrib & 0B00000111), bright);
                        //JJ zx_back_color = zxcolor(((color_attrib & 0B00111000) >> 3), bright);
                        zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                        zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo

                        if (flash && flashing)
                            swap_flash(&zx_fore_color, &zx_back_color);

                        if ((pixel_map & bitpos) != 0)
                        {
                          //if (zx_vidcalc < 268){
                            vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_fore_color);
                          //}
                        }
                        else
                        {
                          //if (zx_vidcalc < 268){
                            vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_back_color);
                          //}
                        }
                    }
                }
            }
        }

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
        videoTaskIsRunning = false;

     time_prev = micros()-time_prev;
     Serial.printf("Tiempo %d\n",time_prev);             
    }
    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {
    }
}
*/



/*
//10500 micros
//Video task optimize
void videoTask(void *unused)
{
    unsigned long time_prev;
    
    unsigned int ff, i, byte_offset;
    unsigned char color_attrib, pixel_map, flash, bright;
    unsigned int zx_vidcalc, calc_y;    

    word zx_fore_color, zx_back_color, tmp_color;    

    videoTaskIsRunning = true;
    uint16_t *param;    
    unsigned char auxColor;
    #define gbvgaMask8Colores 0x3F
    #define gbvgaBits8Colores 0x40
    
    while (1) 
    {        
        xQueuePeek(vidQueue, &param, portMAX_DELAY);
        if ((int)param == 1)
            break;

        time_prev = micros();
        
        for (unsigned int vga_lin = 0; vga_lin < 200; vga_lin++)        
        {
            auxColor = (gb_local_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores;
            if (vga_lin < 3 || vga_lin > 194)
            {
                for (int bor = 32; bor < 328; bor++){
                 vga.backBuffer[vga_lin][bor^2] = auxColor;
                }
            }
            else            
            {
                for (int bor = 32; bor < 52; bor++)
                {
                    vga.backBuffer[vga_lin][bor^2] = auxColor;
                    vga.backBuffer[vga_lin][(bor+276)^2] = auxColor;
                }
                for (ff = 0; ff < 32; ff++) // foreach byte in line
                {
                    byte_offset = (vga_lin - 3) * 32 + ff;
                    calc_y = calcY(byte_offset);
                    if (!video_latch)
                    {
                       color_attrib = local_fast_readbyte(0x5800 + (calc_y / 8) * 32 + ff); // get 1 of 768 attrib values                       
                       pixel_map = local_fast_readbyte(byte_offset + 0x4000);
                     } else
                      {
                       color_attrib = ram7[0x1800 + (calc_y / 8) * 32 + ff]; // get 1 of 768 attrib values                       
                       pixel_map = ram7[byte_offset];//JJ                        
                      }

                    for (i = 0; i < 8; i++) // foreach pixel within a byte
                    {
                        zx_vidcalc = ff * 8 + i;
                        byte bitpos = (0x80 >> i);
                        bright = (color_attrib & 0B01000000) >> 6;
                        flash = (color_attrib & 0B10000000) >> 7;
                        zx_fore_color = gb_local_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                        zx_back_color = gb_local_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo

                        if (flash && flashing)
                            swap_flash(&zx_fore_color, &zx_back_color);

                        if ((pixel_map & bitpos) != 0)
                        {   
                            auxColor = (zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores;
                            vga.backBuffer[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                            //vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_fore_color);
                        }
                        else
                        {                          
                            auxColor = (zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores;
                            vga.backBuffer[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                            //vga.dotFast(zx_vidcalc + 52, calc_y + 3, zx_back_color);                          
                        }
                    }
                }
            }
        }

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
        videoTaskIsRunning = false;
     time_prev = micros()-time_prev;
     Serial.printf("Tiempo %d\n",time_prev);     
    }
    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {
    }
}
*/

//void SDL_Generate_lookup_calcY()
//{//200*32=6400
// for (int i=0;i<6400;i++)
// {  
//  gb_lookup_calcY[i]= calcY(i);
// }
//}

#ifdef use_lib_core_rampa069
 #ifdef use_lib_vga_thread
//Video con hilos
void videoTask(void *unused)
{
    //8300 microsegundos
    //unsigned long time_prev;    
    int byte_offset;
    unsigned char color_attrib, flash;
    //int pixel_map;//, bright;
    int zx_vidcalc, calc_y;
    #ifdef use_lib_ultrafast_vga
     unsigned char ** ptrVGA;
    #endif

    word zx_fore_color, zx_back_color, tmp_color;    

    unsigned char auxColor;
    unsigned char * gb_ptr_ram_1800_video;
    unsigned char * gb_ptr_ram_video;
    unsigned char bitpos;
    #ifdef use_lib_skip_frame
     unsigned char skipFrame=0;
    #endif

    videoTaskIsRunning = true;
    uint16_t *param;
    #ifdef use_lib_ultrafast_vga
     #define gbvgaMask8Colores 0x3F
     #define gbvgaBits8Colores 0x40
     ptrVGA = vga.backBuffer;
    #endif 
    
    while (1) 
    {        
        xQueuePeek(vidQueue, &param, portMAX_DELAY);
        if ((int)param == 1)
            break;

#ifdef use_lib_skip_frame
if (
    (gbFrameSkipVideoCurrentCont < gbFrameSkipVideoMaxCont)
    && 
    (gbFrameSkipVideoMaxCont != 0)
   )
{
 gbFrameSkipVideoCurrentCont++; 
 skipFrame = 1;
}
else
{ 
 gbFrameSkipVideoCurrentCont= 0;
 skipFrame = 0;
}
#endif

#ifdef use_lib_skip_frame
if (skipFrame != 1)
#endif
{

    if (!video_latch){
     gb_ptr_ram_1800_video = &ram5[0x1800];
     gb_ptr_ram_video = ram5;
    }
    else{
     gb_ptr_ram_1800_video = &ram7[0x1800];
     gb_ptr_ram_video = ram7;  
    }    

        //time_prev = micros();     
        for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
        {
            #ifdef use_lib_ultrafast_vga             
             #ifdef use_lib_vga_low_memory
              //Modo bajo y rapido
              #ifdef use_lib_vga8colors
               auxColor = (gb_cache_zxcolor[borderTemp]); //8 colores seguro bajo
              #else
               auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores; //Modo bajo
              #endif 
             #else
              //Modo alto y rapido 
              auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores; //Modo bajo
             #endif 
            #else
             //Modo lento
             auxColor = (gb_cache_zxcolor[borderTemp]);
            #endif
            
            if (vga_lin < 3 || vga_lin > 194)
            {
                #ifdef use_lib_screen_offset
                if ((gb_screen_yIni+vga_lin)>=0)
                #endif
                {                                    
                 #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)                    
                  for (int bor = 0; bor < 296; bor++)
                 #else
                  for (int bor = 32; bor < 328; bor++)
                 #endif
                  {                      
                    //vga.backBuffer[vga_lin][bor^2] = auxColor;
                    //vga.backBuffer[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                    #ifdef use_lib_ultrafast_vga
                     #ifdef use_lib_vga_low_memory
                      //Modo bajo y rapido
                      #ifdef use_lib_vga8colors
                       vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor); //8 colores seguro bajo
                      #else
                       #ifdef use_lib_screen_offset
                        ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)] = auxColor;
                       #else
                        ptrVGA[vga_lin][bor] = auxColor;                      
                       #endif
                      #endif 
                     #else
                      //Modo alto y rapido 
                      #ifdef use_lib_screen_offset
                       ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                      #else
                       ptrVGA[vga_lin][bor^2] = auxColor;
                      #endif
                     #endif
                    #else
                     //Modo lento  
                     #ifdef use_lib_screen_offset
                      vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                     #else
                      vga.dotFast(bor,vga_lin,auxColor);
                     #endif
                    #endif 


/*                    #ifdef use_lib_vga_low_memory
                     vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                    #else
                     #ifdef use_lib_screen_offset
                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                     #else
                      ptrVGA[vga_lin][bor^2] = auxColor;
                     #endif
                    #endif
                    vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                    */
                  }
                }
            }
            else            
            {
               #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)
                for (int bor = 0; bor < 20; bor++)
               #else
                for (int bor = 32; bor < 52; bor++)
               #endif 
                {
                    //vga.backBuffer[vga_lin][bor^2] = auxColor;
                    //vga.backBuffer[vga_lin][(bor+276)^2] = auxColor;
                    //vga.backBuffer[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                    //vga.backBuffer[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)^2] = auxColor;
                  #ifdef use_lib_ultrafast_vga
                   #ifdef use_lib_vga_low_memory
                    //Modo bajo y rapido
                    #ifdef use_lib_vga8colors
                     vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                     vga.dotFast((gb_screen_xIni+bor+276),(gb_screen_yIni+vga_lin),auxColor);                    
                    #else
                     #ifdef use_lib_screen_offset
                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)] = auxColor;
                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)] = auxColor;
                     #else
                      ptrVGA[vga_lin][bor] = auxColor;
                      ptrVGA[vga_lin][(bor+276)] = auxColor;
                     #endif
                    #endif
                   #else
                    //Modo alto y rapido
                    #ifdef use_lib_screen_offset
                     ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                     ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)^2] = auxColor;
                    #else
                     ptrVGA[vga_lin][bor^2] = auxColor;
                     ptrVGA[vga_lin][(bor+276)^2] = auxColor;
                    #endif                    
                   #endif
                  #else
                   //Modo lento
                   #ifdef use_lib_screen_offset
                    vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                    vga.dotFast((gb_screen_xIni+bor+276),(gb_screen_yIni+vga_lin),auxColor);
                   #else
                    vga.dotFast(bor,vga_lin,auxColor);
                    vga.dotFast((bor+276),vga_lin,auxColor);
                   #endif
                  #endif



                    /*#ifdef use_lib_vga_low_memory
                     vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);
                     vga.dotFast((gb_screen_xIni+bor+276),(gb_screen_yIni+vga_lin),auxColor);
                    #else
                     #ifdef use_lib_screen_offset
                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor)^2] = auxColor;
                      ptrVGA[(gb_screen_yIni+vga_lin)][(gb_screen_xIni+bor+276)^2] = auxColor;
                     #else
                      ptrVGA[vga_lin][bor^2] = auxColor;
                      ptrVGA[vga_lin][(bor+276)^2] = auxColor;
                     #endif
                    #endif                    
                   vga.dotFast((gb_screen_xIni+bor),(gb_screen_yIni+vga_lin),auxColor);*/
                }
                byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
                for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
                {                    
                    //calc_y = calcY(byte_offset);                    
                    calc_y = gb_lookup_calcY[(byte_offset>>5)];
                    color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
                    zx_vidcalc = (ff << 3);
                    bitpos = 0x80;
                    for (unsigned char i = 0; i < 8; i++) // foreach pixel within a byte
                    {
                        flash = (color_attrib & 0B10000000) >> 7;
                        #ifdef use_lib_use_bright
                         if (((color_attrib & 0B01000000) >> 6) == 0x01)
                         {
                          zx_fore_color = gb_cache_zxcolor_bright[(color_attrib & 0B00000111)]; //con brillo
                          zx_back_color = gb_cache_zxcolor_bright[((color_attrib & 0B00111000) >> 3)];//con brillo                             
                         }
                         else
                         {
                          zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //sin brillo
                          zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//sin brillo
                         }
                        #else
                         zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                         zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
                        #endif
                        if (flash && flashing)
                        {
                         swap_flash(&zx_fore_color, &zx_back_color);
                        }
                        #ifdef use_lib_ultrafast_vga
                         #ifdef use_lib_vga_low_memory
                          //Modo bajo y rapido
                          #ifdef use_lib_vga8colors
                           auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color; //8 colores seguro bajo
                           #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                            vga.dotFast((gb_screen_xIni+zx_vidcalc + 20),(gb_screen_yIni+(calc_y + 3)),auxColor);
                           #else
                            vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);
                           #endif 
                          #else
                           auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores); //Modo bajo
                           #ifdef use_lib_screen_offset                         
                            #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                             ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 20)] = auxColor;
                            #else
                             ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)] = auxColor;
                            #endif
                           #else
                            #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                             ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)] = auxColor;
                            #else
                             ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)] = auxColor;
                            #endif
                           #endif
                          #endif
                         #else 
                          //Modo alto y rapido
                          auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores);
                          #ifdef use_lib_screen_offset
                           #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                            ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 20)^2] = auxColor;
                           #else
                            ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)^2] = auxColor;
                           #endif 
                          #else                          
                           #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)^2] = auxColor;
                           #else
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                           #endif 
                          #endif
                         #endif
                        #else
                         //Modo lento
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color; //8 colores seguro bajo
                         #ifdef use_lib_screen_offset
                          #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                           vga.dotFast((gb_screen_xIni+zx_vidcalc + 20),(gb_screen_yIni+(calc_y + 3)),auxColor);
                          #else
                           vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);
                          #endif
                         #else
                          #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                           vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                          #else
                           vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor);
                          #endif 
                         #endif
                        #endif                         
                           
                        /*#ifdef use_lib_vga_low_memory
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color;
                        #else
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores);
                        #endif
                        //vga.backBuffer[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                        //vga.backBuffer[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)^2] = auxColor;
                        #ifdef use_lib_vga_low_memory
                         vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);
                        #else                         
                         #ifdef use_lib_screen_offset                         
                          ptrVGA[(gb_screen_yIni+(calc_y + 3))][(gb_screen_xIni+zx_vidcalc + 52)^2] = auxColor;
                         #else
                         #endif
                          ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                        #endif
                        auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color:zx_back_color;
                        vga.dotFast((gb_screen_xIni+zx_vidcalc + 52),(gb_screen_yIni+(calc_y + 3)),auxColor);*/

                        zx_vidcalc++;
                        bitpos = (bitpos>>1);
                    }
                    byte_offset++;
                }
            }
        }
}//fin if skipframe

//gb_sdl_blit=1;

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
        videoTaskIsRunning = false;
     //time_prev = micros()-time_prev;
     //Serial.printf("Tiempo %d\n",time_prev);     
    }
    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {
    }    

}
#else
//Video sin hilos
void videoTaskNoThread()
{
    //8300 microsegundos
    //7680 microsegundos quitando desplazamiento y skip frame
    //unsigned long time_prev;    
    int byte_offset;
    unsigned char color_attrib, flash;
    //int pixel_map;//, bright;
    int zx_vidcalc, calc_y;    

    word zx_fore_color, zx_back_color, tmp_color;    

    unsigned char auxColor;
    unsigned char * gb_ptr_ram_1800_video;
    unsigned char * gb_ptr_ram_video;
    unsigned char bitpos;
    #ifdef use_lib_ultrafast_vga
     unsigned char ** ptrVGA;
     #define gbvgaMask8Colores 0x3F
     #define gbvgaBits8Colores 0x40
     ptrVGA = vga.backBuffer;
    #endif

    if (!video_latch){
     gb_ptr_ram_1800_video = &ram5[0x1800];
     gb_ptr_ram_video = ram5;
    }
    else{
     gb_ptr_ram_1800_video = &ram7[0x1800];
     gb_ptr_ram_video = ram7;  
    }    

        //time_prev = micros();     
        for (int vga_lin = 0; vga_lin < 200; vga_lin++)        
        {            
            #ifdef use_lib_ultrafast_vga
             #ifdef use_lib_vga_low_memory
              //Modo bajo y rapido
              #ifdef use_lib_vga8colors
               auxColor = (gb_cache_zxcolor[borderTemp]); //8 colores seguro bajo
              #else               
               auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores; //Modo bajo
              #endif 
             #else
              //Modo alto y rapido 
              auxColor = (gb_cache_zxcolor[borderTemp] & gbvgaMask8Colores)|gbvgaBits8Colores;
             #endif 
            #else
             //Modo lento
             auxColor = (gb_cache_zxcolor[borderTemp]);
            #endif
                
            if (vga_lin < 3 || vga_lin > 194)
            {
                #if defined(use_lib_vga320x200) || defined(use_lib_vga320x240)                 
                 for (int bor = 0; bor < 296; bor++)
                #else 
                 for (int bor = 32; bor < 328; bor++)                
                #endif
                {                    
                  #ifdef use_lib_ultrafast_vga
                   #ifdef use_lib_vga_low_memory
                    //Modo bajo y rapido
                    #ifdef use_lib_vga8colors
                     vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                    #else 
                     ptrVGA[vga_lin][bor] = auxColor; //Modo bajo
                    #endif 
                   #else
                    //Modo alto y rapido
                    ptrVGA[vga_lin][bor^2] = auxColor;
                   #endif
                  #else
                   //Modo lento                   
                   vga.dotFast(bor,vga_lin,auxColor);
                  #endif
                }                
            }
            else            
            {
                #if defined(use_lib_vga320x200) || defined (use_lib_vga320x240)                 
                 for (int bor = 0; bor < 20; bor++)
                #else
                 for (int bor = 32; bor < 52; bor++)
                #endif               
                {                    
                 #ifdef use_lib_ultrafast_vga
                  #ifdef use_lib_vga_low_memory
                   //Modo bajo y rapido
                   #ifdef use_lib_vga8colors
                    vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                    vga.dotFast((bor+276),vga_lin,auxColor);
                   #else 
                    ptrVGA[vga_lin][bor] = auxColor; //Modo bajo
                    ptrVGA[vga_lin][(bor+276)] = auxColor;
                   #endif
                  #else
                   //Modo alto y rapido
                   ptrVGA[vga_lin][bor^2] = auxColor;
                   ptrVGA[vga_lin][(bor+276)^2] = auxColor;
                  #endif
                 #else
                  //Modo lento
                  vga.dotFast(bor,vga_lin,auxColor); //8 colores seguro bajo
                  vga.dotFast((bor+276),vga_lin,auxColor);
                 #endif
                }
                byte_offset = ((vga_lin - 3)<<5); //optimizado (vga_lin - 3) * 32
                for (unsigned char ff = 0; ff < 32; ff++) // foreach byte in line
                {                    
                    //calc_y = calcY(byte_offset);                    
                    calc_y = gb_lookup_calcY[(byte_offset>>5)];
                    color_attrib = gb_ptr_ram_1800_video[(((calc_y >> 3) << 5) + ff)]; // get 1 of 768 attrib values                    
                    zx_vidcalc = (ff << 3);
                    bitpos = 0x80;
                    for (unsigned char i = 0; i < 8; i++) // foreach pixel within a byte
                    {
                        flash = (color_attrib & 0B10000000) >> 7;
                        zx_fore_color = gb_cache_zxcolor[(color_attrib & 0B00000111)]; //falta brillo
                        zx_back_color = gb_cache_zxcolor[((color_attrib & 0B00111000) >> 3)];//falta brillo
                        if (flash && flashing)
                        {
                         swap_flash(&zx_fore_color, &zx_back_color);
                        }
                        #ifdef use_lib_ultrafast_vga
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? ((zx_fore_color & gbvgaMask8Colores)|gbvgaBits8Colores):((zx_back_color & gbvgaMask8Colores)|gbvgaBits8Colores);
                        #else
                         auxColor = ((gb_ptr_ram_video[byte_offset] & bitpos) != 0)? zx_fore_color : zx_back_color;
                        #endif

                        
                        #if defined(use_lib_vga320x240) || defined(use_lib_vga320x200)
                         //320x240
                         #ifdef use_lib_ultrafast_vga
                          #ifdef use_lib_vga_low_memory
                           //Modo bajo y rapido
                           #ifdef use_lib_vga8colors
                            vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                           #else 
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)] = auxColor;
                           #endif
                          #else
                           //Modo alto y rapido  
                           ptrVGA[(calc_y + 3)][(zx_vidcalc + 20)^2] = auxColor;
                          #endif                            
                         #else
                          //Modo lento
                          vga.dotFast((zx_vidcalc + 20),(calc_y + 3),auxColor);
                         #endif                         
                        #else
                         //360x200
                         #ifdef use_lib_ultrafast_vga
                          #ifdef use_lib_vga_low_memory
                           //Modo bajo y rapido
                           #ifdef use_lib_vga8colors
                            vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor); //8 colores seguro bajo
                           #else
                            ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)] = auxColor;  //Modo bajo
                           #endif
                          #else
                           //Modo alto y rapido
                           ptrVGA[(calc_y + 3)][(zx_vidcalc + 52)^2] = auxColor;
                          #endif                           
                         #else
                          //Modo lento 
                          vga.dotFast((zx_vidcalc + 52),(calc_y + 3),auxColor);
                         #endif
                        #endif 

                        zx_vidcalc++;
                        bitpos = (bitpos>>1);
                    }
                    byte_offset++;
                }
            }
        }
        //gb_sdl_blit=1;
     //time_prev = micros()-time_prev;
     #ifdef use_lib_log_serial
      //Serial.printf("Tiempo %d\n",time_prev);             
     #endif
}
 #endif
#endif



void swap_flash(word *a, word *b) {
    word temp = *a;
    *a = *b;
    *b = temp;
}





// Load zx keyboard lines from PS/2
void do_keyboard() {
    byte kempston = 0;

  #ifdef use_lib_remap_keyboardpc
   //Cursores menu en 128k
   if (keymap[KEY_CURSOR_UP] == 0){ 
    keymap[0x12]= keymap[0x3d] = 0; //shift+7
   }
   else
   {
    if (keymap[KEY_CURSOR_DOWN] == 0) { keymap[0x12]= keymap[0x36]= 0; }
    //else {keymap[0x12]= keymap[0x36]= 1;}//shift+6   
   }
  #endif 
     

    bitWrite(z80ports_in[0], 0, keymap[0x12]);
    bitWrite(z80ports_in[0], 1, keymap[0x1a]);
    bitWrite(z80ports_in[0], 2, keymap[0x22]);
    bitWrite(z80ports_in[0], 3, keymap[0x21]);
    bitWrite(z80ports_in[0], 4, keymap[0x2a]);

    bitWrite(z80ports_in[1], 0, keymap[0x1c]);
    bitWrite(z80ports_in[1], 1, keymap[0x1b]);
    bitWrite(z80ports_in[1], 2, keymap[0x23]);
    bitWrite(z80ports_in[1], 3, keymap[0x2b]);
    bitWrite(z80ports_in[1], 4, keymap[0x34]);

    bitWrite(z80ports_in[2], 0, keymap[0x15]);
    bitWrite(z80ports_in[2], 1, keymap[0x1d]);
    bitWrite(z80ports_in[2], 2, keymap[0x24]);
    bitWrite(z80ports_in[2], 3, keymap[0x2d]);
    bitWrite(z80ports_in[2], 4, keymap[0x2c]);

    bitWrite(z80ports_in[3], 0, keymap[0x16]);
    bitWrite(z80ports_in[3], 1, keymap[0x1e]);
    bitWrite(z80ports_in[3], 2, keymap[0x26]);
    bitWrite(z80ports_in[3], 3, keymap[0x25]);
    bitWrite(z80ports_in[3], 4, keymap[0x2e]);

    bitWrite(z80ports_in[4], 0, keymap[0x45]);
    bitWrite(z80ports_in[4], 1, keymap[0x46]);
    bitWrite(z80ports_in[4], 2, keymap[0x3e]);
    bitWrite(z80ports_in[4], 3, keymap[0x3d]);
    bitWrite(z80ports_in[4], 4, keymap[0x36]);

    bitWrite(z80ports_in[5], 0, keymap[0x4d]);
    bitWrite(z80ports_in[5], 1, keymap[0x44]);
    bitWrite(z80ports_in[5], 2, keymap[0x43]);
    bitWrite(z80ports_in[5], 3, keymap[0x3c]);
    bitWrite(z80ports_in[5], 4, keymap[0x35]);

    bitWrite(z80ports_in[6], 0, keymap[0x5a]);
    bitWrite(z80ports_in[6], 1, keymap[0x4b]);
    bitWrite(z80ports_in[6], 2, keymap[0x42]);
    bitWrite(z80ports_in[6], 3, keymap[0x3b]);
    bitWrite(z80ports_in[6], 4, keymap[0x33]);

    bitWrite(z80ports_in[7], 0, keymap[0x29]);
    bitWrite(z80ports_in[7], 1, keymap[0x14]);
    bitWrite(z80ports_in[7], 2, keymap[0x3a]);
    bitWrite(z80ports_in[7], 3, keymap[0x31]);
    bitWrite(z80ports_in[7], 4, keymap[0x32]);

    //Serial.printf("Teclado %d %d\n",keymap[KEY_CURSOR_UP],keymap[KEY_CURSOR_DOWN]);
    // Kempston joystick
    z80ports_in[0x1f] = 0;
    bitWrite(z80ports_in[0x1f], 0, !keymap[KEY_CURSOR_RIGHT]);
    bitWrite(z80ports_in[0x1f], 1, !keymap[KEY_CURSOR_LEFT]);
    bitWrite(z80ports_in[0x1f], 2, !keymap[KEY_CURSOR_DOWN]);
    bitWrite(z80ports_in[0x1f], 3, !keymap[KEY_CURSOR_UP]);
    bitWrite(z80ports_in[0x1f], 4, !keymap[KEY_ALT_GR]);    

 #ifdef use_lib_remap_keyboardpc
  if (keymap[KEY_CURSOR_UP] == 0){ 
   keymap[0x12]= keymap[0x3d] = 1; //shift+7
  }
  else
  {
   if (keymap[KEY_CURSOR_DOWN] == 0) { keymap[0x12]= keymap[0x36]= 1; }
   //else {keymap[0x12]= keymap[0x36]= 1;}//shift+6   
  }
 #endif 
}

#ifdef use_lib_remap_keyboardpc 
 //Cursores menu en 128k
 void do_keyboard_remap_pc()
 { 
  if (checkAndCleanKey(KEY_BACKSPACE) || checkAndCleanKey(KEY_DELETE))
   keymap[0x12]= keymap[0x45]=0;  //shift+0
   
  //if (checkKey(KEY_CURSOR_UP))
  // keymap[0x12]= keymap[0x3d] = 0; //shift+7    

  //if (checkKey(KEY_CURSOR_DOWN))
  // keymap[0x12]= keymap[0x36] = 0;  //shift+6
 }
#endif 


// +-------------+
// | LOOP core 1 |
// +-------------+
//
void loop() {
    // static byte last_ts = 0;
    //unsigned long ts1, ts2;

    if (halfsec) {
        flashing = ~flashing;
    }
    sp_int_ctr++;
    halfsec = !(sp_int_ctr % 25);

    gb_currentTime = millis();
    if ((gb_currentTime-gb_keyboardTime) >= gb_current_ms_poll_keyboard)
    {
     gb_keyboardTime = gb_currentTime;         
     #ifdef use_lib_remap_keyboardpc 
      do_keyboard_remap_pc();
     #endif 
     do_keyboard();
    }
    do_tinyOSD();
            
    if ((gb_current_delay_emulate_ms == 0) || (gb_run_emulacion == 1))
    {//Ejecutamos emulacion z80    
     // ts1 = millis();         
     #ifdef use_lib_core_rampa069        
      zx_loop();
     #else
      loop_jsanchezv();
     #endif      
     // ts2 = millis();     
    }    

    if (gb_current_delay_emulate_ms != 0)
    {
     if (gb_sdl_blit == 1)
     {
      gb_sdl_blit= 0;
      gb_run_emulacion= 0;
      gb_time_ini_espera = millis();
     }
    }
    
   
    #ifdef use_lib_sound_ay8912
     if (gb_silence_all_channels == 1)
      SilenceAllChannels();
     else 
     {
      gb_currentTime = millis();
      if ((gb_currentTime-gb_sdl_time_sound_before) >= gb_current_ms_poll_sound)
      {             
       gb_sdl_time_sound_before= gb_currentTime;
       jj_mixpsg();
       sound_cycleFabgl();
      }
     }
    #endif 

    #ifdef use_lib_mouse_kempston
     PollMouse();
    #endif 

    #ifdef use_lib_core_rampa069
     #ifdef use_lib_vga_thread
      xQueueSend(vidQueue, &param, portMAX_DELAY);
      while (videoTaskIsRunning)
      {
      }
      gb_sdl_blit = 1;
     #else
      gbTimeVideoNow = millis();
      if ((gbTimeVideoNow - gbTimeVideoIni) >= gbDelayVideo)     
      {
       gbTimeVideoIni = gbTimeVideoNow;
       videoTaskNoThread();
       gb_sdl_blit= 1;      
      }
     #endif
    #endif 


    gb_currentTime = millis();     
    if (gb_run_emulacion == 0)    
     if ((gb_currentTime - gb_time_ini_espera) >= (gb_current_delay_emulate_ms))
      gb_run_emulacion = 1;


//videoTaskNoThread(); //Video sin hilos

    
    //if ((ts2 - ts1) != last_ts) {
    //    Serial.printf("PC:  %d time: %d\n", _zxCpu.pc, ts2 - ts1);
    //    last_ts = ts2 - ts1;
    //}

    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
    vTaskDelay(0); // important to avoid task watchdog timeouts - change this to slow down emu
}
