# Tiny ESP32 ZX Spectrum
Fork of David Crespo's emulator with the support of WII <a href='https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote'>Wiimote</a>, based on the emulator of Ramon Martinez and Jorge Fuertes <a href='https://github.com/rampa069/ZX-ESPectrum'>ZX-ESPectrum</a>, with dependencies of the author Pete Todd.
<br>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewZx128k.gif'></center>
I have made several modifications:
<ul>
 <li>PSRAM is not used, it works in ESP32 with 520 KB of RAM (TTGO VGA32 v1.x)</li>
 <li>Supports 128K,+2,+3 and +3e modes without lack of PSRAM</li>
 <li>Removed the control support WII</li>
 <li>Replacement of the OSD with a low resource OSD</li>
 <li>Created project compatible with Arduino IDE and Platform IO</li>
 <li>Removal of SPIFFS support</li>
 <li>ROM, SNA, SCR, TAP repositioning in Flash progmem</li>
 <li>Size optimization in FLASH</li>
 <li>Video routine optimization (2200 microseconds per frame)</li>
 <li>Support for both 2 core's (50 fps 20000 microseconds) and one core (50 fps 17000 microseconds) modes.</li>
 <li>Kempston mouse emulation with fabgl</li>
 <li>Kempston mouse emulation (reduced library by Rob Kent jazzycamel)</li>
 <li>Kempston emulation with cursor keys (left, right, up, down) and right ALT.</li>
 <li>AY8912 emulation with fabgl, reduced version of dcrespo3d (David Crespo Tascón)</li>
 <li>Redirecting tape recording pulses to the speaker</li>
 <li>Resample option in speaker mode and mix with AY8912 (unstable)</li>
 <li>Possibility to choose brightness mode and 8 color modes in compilation.</li>
 <li>DAC VGA 64 colors (6 bits) and 8 colors (3 bits) (tiny version Ricardo Massaro)</li>
 <li>Removal of the cursors in the menu of +2,+3, as well as the key of erasing</li>
 <li>X, Y screen adjustment (slightly CPU consuming)</li>
 <li>R, G, B color scale, green phosphorous screen style</li>
 <li>Emulation video speed menu</li>
 <li>AUTO emulation speed menu (50 fps 20000 microseconds) as well as a delay in microseconds or without brake, from 150 fps to 396 fps in Lin Ke-Fong's core or from 95 fps to 225 fps in Jose Luis Sanchez's core.</li> 
 <li>Support for reading SCR files</li>
 <li>Support for loading screen files into TAP files</li>
 <li>BASIC loading from .TAP (tape routine intercept). Only in Lin Ke-Fong core</li>
 <li>Core support from Lin Ke-Fong and José Luis Sánchez</li>
 <li>Created Windows port under SDL (not available for download)</li>
 <li>Video modes 360x200, 320x240, 320x200</li>
 <li>128K SNA support</li>
</ul>

<br><br>
<h1>Requirements</h1>
Required:
 <ul>
  <li>TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)</li>
  <li>Visual Studio 1.48.1 PLATFORMIO 2.2.1 Espressif32 v3.3.2</li>
  <li>Arduino IDE 1.8.11 Espressif System 1.0.6</li>
  <li>Arduino fabgl 0.9.0 reduced library (included in PLATFORMIO project)</li>
  <li>Arduino bitluni 0.3.3 reduced library (included in project)</li>
 </ul>
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/ttgovga32v12.jpg'></center>
<br><br>
 

<h1>PlatformIO</h1>
PLATFORMIO 2.2.1 must be installed from the Visual Studio extensions. Espressif32 v3.3.2 is also required.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIOinstall.gif'></center>
Then the working directory <b>TinyZXESPectrumttgovga32</b> will be selected.
We must modify the file <b>platformio.ini</b> the option <b>upload_port</b> to select the COM port where we have our TTGO VGA32 board.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIO.gif'></center>
Then we will proceed to compile and upload to the board. No partitions are used, so we must upload the whole compiled binary. It's all set up so we don't have to install the bitluni and fabgl libraries.


<br><br>
<h1>Arduino IDE</h1>
The whole project is compatible with the structure of Arduino 1.8.11.
We only have to open the <b>ZXESPectrum.ino</b> from the <b>ZXESPectrum</b> directory.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewArduinoIDEpreferences.gif'></center>
We must install the spressif extensions in the additional card url manager <b>https://dl.espressif.com/dl/package_esp32_index.json</b>
The project is now ready, so no bitluni or fabgl libraries are needed. We must deactivate the PSRAM option, and in case of exceeding 1 MB of binary, select 4 MB of partition when uploading. Although the code does not use PSRAM, if the option is active and our ESP32 does not have it, an exception will be generated and it will restart in loop mode.



<br><br>
<h1>Usability</h1>
Loading is allowed:
 <ul>
  <li>48K and 128K ROMS</li>
  <li>SNA of 48K</li>
  <li>Read screenshots of .SCR files. The .SCR file, converted to hexadecimal, contains an image, which is dumped into the video memory area. SCR files are very easy to get, if we use the emulator <b>fuse</b>.</li>
  <li>Read tape screens .TAP. We should have a hexadecimal converted .TAP file with the basic BASIC loader format, followed by a SCREEN data block. The routine that I have done is very basic, and it is in charge of looking for the screen block, and dumping it into video memory.</li>
  <li>Select TAP allows you to select the .TAP and place it in block 0, in order to read the BASIC from a tape. This way, if we select the <b>Tape loader</b> from the 128K menu, or if we type the <b>load ""</b> command from the BASIC, it will proceed to load the BASIC code. We must have a .TAP file converted in hexadecimal with the basic loader format and BASIC code. It is only active for the Lin Ke-Fong core, and only loads the first block of a BASIC program. If that BASIC program ends up launching a RUN or a LOAD, it will not be able to process that new data load, causing a restart or something random.

Translated with www.DeepL.com/Translator (free version)</li>
 </ul>
 There is a basic OSD of low resources, that is to say, very simple, that is visualized by pressing the key <b>F1</b>.
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewOSD.gif'></center>
 Files must be converted to .h in hexadecimal. You can use the online tool:<br>
 <a href='http://tomeko.net/online_tools/file_to_hex.php?lang=en'>http://tomeko.net/online_tools/file_to_hex.php?lang=en</a>
 
 
<br><br>
<h1>Options</h1>
The file <b>gbConfig.h</b> options are selected:
<ul>
 <li><b>use_lib_mouse_kempston:</b> It uses a mouse connected to the PS/2 port emulating the kempston protocol. Rob Kent's reduced library is used. By using an uninterruptible mode, CPU is consumed in each iteration, reducing emulation frames.</li>
 <li><b>define use_lib_mouse_kempston </b> If active, it allows to choose by default the mouse in left-handed mode. At any time, from the OSD, you can also change it, as well as choose the inversion of the X and Y, and the speed of increase.</li>
 <li><b>gb_delay_init_ms: </b> Allows you to specify the milliseconds to wait for mouse initialization.</li>
 <li><b>gb_ms_mouse: </b> Allows you to specify the mouse polling milliseconds.</li>
 <li><b>use_lib_sound_ay8912:</b> A 3-channel mixer is used in dirty mode, emulating the AY8912. The speaker uses channel 1 in digital mode, unless the resampling option is activated. The reduced fabgl library is required, which is already inside the project.</li>
 <li><b>use_lib_resample_speaker:</b> The speaker output is emulated as a sine wave on channel 1. It is still in a very experimental state.</li>
 <li><b>use_lib_redirect_tapes_speaker:</b> Redirects the tone output from the tape to the speaker.</li>
 <li><b>use_lib_vga8colors:</b> Forces to use RGB, only 3 pins and no brightness mode, 8 colors</li>
 <li><b>use_lib_vga64colors:</b> Forced to use RRGGBB, 6-pin, no brightness mode, i.e. with 8 colors</li>
 <li><b>use_lib_use_bright:</b> Force to use RRGGBB, 6 pins, with brightness mode, 16 colors</li>
 <li><b>use_lib_vga_low_memory:</b> Experimental mode with low video RAM consumption. In some video mode it gives problems.</li>
 <li><b>use_lib_vga_thread:</b> Use the video output in a thread. If it is annotated, only one core will be used.</li>
 <li><b>use_lib_screen_offset:</b> Allows to move left and up the screen. It consumes a little CPU, a couple of microseconds. If active, it can be adjusted from the OSD. Ideally, do not have it active and make the adjustments from the monitor.</li>
 <li><b>use_lib_skip_frame:</b> Allows to skip frames when emulating</li>
 <li><b>use_lib_vga360x200:</b> Normal video mode</li>
 <li><b>use_lib_vga320x200:</b> Video mode 320x200</li> 
 <li><b>use_lib_vga320x240:</b> Video mode 320x240</li>
 <li><b>use_lib_log_serial:</b> Logs are sent by serial port usb</li>
 <li><b>gb_ms_keyboard:</b> The number of milliseconds of polling must be specified for the keyboard.</li>
 <li><b>gb_ms_sound:</b> The number of polling milliseconds must be specified for sound AY8912.</li>
 <li><b>gb_frame_crt_skip:</b> The number of video frames to be skipped</li>
 <li><b>gb_delay_emulate_ms:</b> Milliseconds of waiting for each completed frame.</li> 
 <li><b>use_lib_tape_rom_intercept:</b> Controls the 0x056B routine to read tapes. It is only active for the Lin Ke-Fong core, and only loads the first block of a BASIC program. It consumes a few CPU cycles, as it is checking the PC register.</li>
 <li><b>use_lib_ultrafast_speaker:</b> Fast mode of audio I/O pin activation.</li> 

 <li><b>use_lib_stats_time_unified: </b> It allows to see traces through the serial port with the fps, as well as current microseconds, minimum and maximum per frame. The same for video.</li>
 <li><b>use_lib_delay_tick_cpu_auto: </b> If it is set to 1, it allows you to set the frame rate to 20000 microseconds, i.e. 20 milliseconds, which is equivalent to 50 fps. Subsequently from the OSD you can change the CPU standby speed.</li>
 <li><b>use_lib_delay_tick_cpu_micros: </b> If 0, no wait is applied at each instruction execution.</li>
 <li><b>use_lib_cpu_adjust_mask_instructions: </b> It applies a time reset mask for the 20000 microseconds of each frame. If the mask is 0x03, the binary mask 00000011 is applied, i.e. every 4 instructions.</li>
 <li><b>use_lib_core_linkefong: </b> Allows to choose the Lin Ke-Fong core. Once compiled, it will be displayed on the OSD.</li>
 <li><b>use_lib_core_jsanchezv: </b> It allows to choose the core of José Luis Sánchez. Once compiled, it will be displayed on the OSD.</li>
 <li><b>use_lib_delayContention: </b> If active, it allows to apply the wait of the contained memory, only in the Jose Luis Sanchez core.</li>
</ul>


<br><br>
<h1>Brightness mode</h1>
The brightness mode corresponds to 16 colors, so the 6-pin DAC output must be used. This mode is incompatible with the 8 colors mode, so if you choose this option, you have to disable the <b>use_lib_vga8colors</b> in the gbConfig.h.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewFantasy.gif'></center>
This mode consumes more CPU and RAM than the normal 8-color mode.


<br><br>
<Test applications
When starting the ESP32, it is done in ZX 48K mode, that is, loading the rom of the ZX 48k with the BASIC.
Every time we select a ROM, the tape selection is reset to block 0, as well as the sound.
Later we can choose the roms:
<ul>
 <li>
  <b>ROMS of 48K</b>
  <ul>
   <li><b>DIAG:</b> diagnostic utility, for keyboard, ram, rom, 48k, 128k, AY8912 mode, speaker and video test.</li>
   <li><b>SE:</b> Open SE rom BASIC, which allows you to use BASIC without the 48K mode function keys, so that each keystroke does not generate a function.</li>
   <li><b>SINCLAIR:</b> The 48k mode</li>
  </ul>
 </li>
 <li>
  <b>ROMS of 128K</b>
  <ul>
   <li><b>PLUS2A:</b> The spectrum +2</li>
   <li><b>PLUS3:</b> The spectrum +3</li>
   <li><b>PLUS3E:</b> The spectrum +3E</li>
   <li><b>SINCLAIR:</b> The 128K</li>
  </ul>
 </li> 
</ul>
<br>
In the case of the SNA:
<ul> 
 <li><b>DIAG:</b> ROM and RAM diagnostic utility.</li>
 <li><b>Fantasy: </b>Gloss support game (16 colors) It allows kempston, which together with the mouse, can be played very comfortably.</li>
 <li><b>Sppong: </b> The classic pong</li>
 <li><b>Fire: </b> It is a modified version of Fire, to support kempston mouse, so if you play without it, it will give problems. You can go to the main menu by pressing shift and the space bar. It has support for AY8912, however, the speaker output at the same time as AY8912 is not yet 100% effective. Separately, it is correct.</li>
 <li><b>3DcoolKm:</b> 3D tool with kempston mouse support</li>
 <li><b>AT4openKM: </b> Drawing tool with kempston mouse support</li>
 <li><b>Sipka: </b> Kempston mouse test tool Allows to change the mouse icon by clicking on the buttons.</li>
 <li><b>MouseTestZXds:</b> Tool for testing the kempston mouse protocol</li>
 <li><b>FireKey:</b> The same Fire game but using the keyboard instead of the mouse. It has sound output AY8912 (128K mode) and internal speaker (48K mode).</li>
 <li><b>Tritone1:</b> Musicdisk that uses internal speaker.</li> 
 <li><b>Z80FULL: </b> The complete z80 test. It must be executed with the <b>RUN</b> command.</li>
</ul>
<br>
For SCR loading from TAP:
<ul>
 <li><b>3D:</b> It doesn't load anything, because this tape only has the BASIC code of 3D.</li>
 <li><b>4x4:</b> Load the game car 4x4</li> 
</ul>
<br>
For SCRs:
<ul>
 <li><b>4x4:</b> Load the image of the game 4x4.</li>
</ul>
<br>
For loading tapes from BASIC or Tape Loader:
<ul>
 <li><b>3D:</b> Loads a 3D graphics simulation.</li>
</ul>


<br><br>
<h1>Tool data2h</h1>
I have created a very basic tool, to convert .rom, .tape, .sna, .scr files into .h in ready mode to be processed by the emulator. We only have to leave the files in the folder <b>input</b> in each type of folder and execute the file <b>data.exe</b>, so that an output will be generated in the directory <b>output/dataFlash</b>. For testing purposes, several files have been left in each directory, which it is recommended to delete in case of making a new custom list. It is also recommended to delete the files in the <b>ESP32TinyZXSpectrum\dataFlash\</b> directory in order to have a clean project.<br><br>
<a href='https://github.com/rpsubc8/ESP32TinyZXSpectrum/tree/main/tools/data2h'>Tool data2h</a>
<br><br>
<pre>
 input/
  roms/
   48k/
    jupiter/
     0.rom - ROM Jupiter Ace
    sinclair/
     0.rom - ROM ZX48K 
   128k/
    305zx80IF2/
     0.rom - ROM ZX-80 ROM
    308zx81IF2/
     0.rom - ROM ZX-81 ROM
    SINCLAIR128/
     0.rom - ZX-128 ROM
     1.rom
  scr/
   trixs01.scr
   trixs02.scr
   trixs03.scr
  sna/
   48k/
    coloristic.sna
    GDC.sna
   128k/
    nothing.txt
</pre>
The concept of numerical rom files (0.rom, 1.rom, 2.rom, 3.rom) follows the same basis as the ZXESPectrum emulator. The name of the folder is what defines the title of rom.
It is mandatory to leave the folder <b>48k/sinclair/0.rom</b>, because you need the 48k rom to start the emulator.
Then we must copy the directory <b>dataFlash</b> into the project <b>ESP32TinyZXSpectrum\ZXESPectrum</b> overwriting the previous dataFlash folder. It is recommended to clean up the project and recompile it.<br>
This tool is very simple, and does not control errors, so it is recommended to leave you the files with very simple names and as simple as possible.<br>
The project in PLATFORM.IO is prepared for 2 MB of Flash. If we need the 4MB of flash, we will have to modify the entry in the file <b>platform.ini</b>
<pre>board_build.partitions = huge_app.csv</pre>
In the Arduino IDE, we must choose the option <b>Partition Scheme (Huge APP)</b>.


<br><br>
<h1>SNA format</h1>
The 48K (49179 bytes) and 128K (131103 bytes) SNA format is supported, compatible with FUSE emulator SNAs.<br>
The 48K SNA format:
<pre>
 Offset   Size   Description
 ------------------------------------------------------------------------
 0        1      byte   I
 1        8      word   HL',DE',BC',AF'
 9        10     word   HL,DE,BC,IY,IX
 19       1      byte   Interrupt (bit 2 contains IFF2, 1=EI/0=DI)
 20       1      byte   R
 21       4      words  AF,SP
 25       1      byte   IntMode (0=IM0/1=IM1/2=IM2)
 26       1      byte   BorderColor (0..7, not used by Spectrum 1.7)
 27       49152  bytes  RAM dump 16384..65535
 ------------------------------------------------------------------------
 Total: 49179 bytes
</pre>

The 128K SNA format:
<pre>
 Offset   Size   Description
 ------------------------------------------------------------------------
 0        27     bytes  SNA header (see above)
 27       16Kb   bytes  RAM bank 5 \
 16411    16Kb   bytes  RAM bank 2  } - as standard 48Kb SNA file
 32795    16Kb   bytes  RAM bank n / (currently paged bank)
 49179    2      word   PC
 49181    1      byte   port 0x7ffd setting
 49182    1      byte   TR-DOS rom paged (1) or not (0)
 49183    16Kb   bytes  remaining RAM banks in ascending order
 ...
 ------------------------------------------------------------------------
 Total: 131103 bytes
</pre>
The id of the cached bank (bank n) corresponds to the 3 least significant bits of the port (0x7ffd & 0x07).
After the 49183 byte of the SNA, go the 8 banks of 16384 bytes, eliminating bank 5, 2 and the currently cached one (bank n).
