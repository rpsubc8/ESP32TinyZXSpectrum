# Tiny ESP32 ZX Spectrum
Fork of David Crespo's emulator with the support of WII <a href='https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote'>Wiimote</a>, based on the emulator of Ramon Martinez and Jorge Fuertes <a href='https://github.com/rampa069/ZX-ESPectrum'>ZX-ESPectrum</a>, with dependencies of the author Pete Todd.
<br>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewZx128k.gif'></center>
I have made several modifications:
<ul>
 <PSRAM is not used, it works in ESP32 with 520 KB of RAM (TTGO VGA32)</li> 
 <Supports 128K,+2,+3 and +3e modes without lack of PSRAM
 <li>Removed the control support WII</li>
 <Replacement of the OSD with a low resource OSD
 <Created project compatible with Arduino IDE and Platform IO
 <Removal of SPIFFS support
 <ROM, SNA, SCR, TAP repositioning in Flash progmem
 <Size optimization in FLASH 
 <Optimization of the video routine
 <Support for wireless mode 
 <Kempston mouse emulation with fabgl  
 <Emulation of AY8912
 <li>Redirecting tape recording pulses to the speaker</li>
 <Resample option in speaker mode and mix with AY8912 (unstable)</li>
 <Possibility to choose brightness mode and 8 color modes in compilation.
 <li>Removal of the cursors in the menu of +2,+3, as well as the key of erasing</li>
 <Screen adjustment X, Y</li>
 <R, G, B color scale, green phosphorous screen style
 <Emulation video speed menu
 <Support for reading SCR files
 <Support for loading screen files into TAP files
 <Load BASIC from TAP (tape intercept routine) 
 <Created Windows port under SDL</li>  
</ul>

<br>
<Requirements
Required:
 <ul>
  <Visual Studio 1.48.1 PLATFORM 2.2.0
  <li>Arduino IDE 1.8.11</li>
  <li>Arduino fabgl bookstore 0.9.0</li>
  <li>Arduino bitluni bookcase 0.3.3</li>
 </ul>
 

<h1>PlatformIO</h1>
PLATFORM 2.2.0 must be installed from the Visual Studio extensions.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIOinstall.gif'></center>
Then the working directory <b>TinyZXESPectrumttgovga32</b> will be selected.
We must modify the file <b>platformio.ini</b> the option <b>upload_port</b> to select the COM port where we have our TTGO VGA32 board.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIO.gif'></center>
Then we will proceed to compile and upload to the board. No partitions are used, so we must upload all the compiled binary.
Everything is prepared so we don't have to install the bitluni libraries. However, if we need the sound and mouse options, we will need to install the libraries of <b>fabgl</b>.


<br>
<h1>Arduino IDE</h1>
The whole project is compatible with the structure of Arduino 1.8.11.
We only have to open the <b>ZX-ESPectrum.ino</b> from the <b>ZX-ESPectrum</b> directory.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewArduinoIDEpreferences.gif'></center>
We must install the spressif extensions in the additional card url manager <b>https://dl.espressif.com/dl/package_esp32_index.json</b>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewArduinoIDElibrary.gif'></center>
For the normal mode, the project is already prepared, so that no bitluni library is needed. However, if we need the mouse and sound support, it is required to install the libraries <b>fabgl 0.9.0</b>.
We can do it from the library manager.
We must deactivate the PSRAM option, and in case of exceeding 1 MB of binary, select 4 MB of partition at the time of uploading. Although the code does not use PSRAM, if the option is active and our ESP32 does not have it, an exception will be generated and restart it in loop mode.




<br>
<h1>Usability</h1>
Loading is allowed:
 <ul>
  <48K and 128K ROMS
  <li>SNA of 48K</li>
  <li>Read screenshots of .SCR files. The .SCR file, converted to hexadecimal, contains an image, which is dumped into the video memory area. SCR files are very easy to get, if we use the emulator <b>fuse</b>.
  <li>Read tape screens .TAP. We should have a hexadecimal converted .TAP file with the basic BASIC loader format, followed by a SCREEN data block. The routine that I have done is very basic, and it is in charge of looking for the screen block, and dumping it into video memory</li>.
  <Select TAP to choose the TAP and go to block 0, to read the BASIC of a tape. This way, if we select the <b>Tape loader</b> from the 128K menu, or if we write the command <b>load ""</b> from the BASIC, the BASIC code will be loaded. We should have a .TAP file converted to hexadecimal with the basic format of loader and BASIC code.</li>
 </ul>
 There is a basic OSD of low resources, that is to say, very simple, that is visualized by pressing the key <b>F1</b>.
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewOSD.gif'></center>
 Files must be converted to .h in hexadecimal. You can use the online tool:<br>
 <a href='http://tomeko.net/online_tools/file_to_hex.php?lang=en'>http://tomeko.net/online_tools/file_to_hex.php?lang=en</a>
 
 
<br>
<Options
The file <b>gbConfig.h</b> options are selected:
<ul>
 <li><b>use_lib_mouse_kempston:</b> A mouse connected to the PS/2 port emulating the kempston protocol is used. The fabgl library 0.9.0</li> is used
 <li><b>use_lib_sound_ay8912:</b> A 3 channel mixer is used in dirty mode, emulating AY8912. The speaker uses channel 1 in digital mode, unless the resample option is enabled. The fabgl library 0.9.0</li> is required.
 <li><b>use_lib_resample_speaker:</b> The speaker output is emulated as a sine wave on channel 1. The fabgl library 0.9.0</li> is required
 <li><b>use_lib_redirect_tapes_speaker:</b> Redirects the tone output from the tape to the speaker.</li>
 <li><b>use_lib_vga8colors:</b> Force RGB, only 3 pins and no brightness mode, 8 colors</li>
 <li><b>use_lib_use_bright:</b> Force to use RRGGBB, 6 pins, with brightness mode, 16 colors</li>
 <li><b>use_lib_vga_low_memory:</b> Experimental mode with low video RAM consumption</li>
 <li><b>use_lib_vga_thread:</b> Use the video output in a thread. If it is done in polling mode, neither the screen adjustment nor the brightness modes will be used. The polling mode is intended for debugging</li>
 <li><b>use_lib_screen_offset:</b> Allows to move to the left and up the screen.</li>
 <li><b>use_lib_skip_frame:</b> Allows to skip frames when emulating</li>
 <li>>use_lib_vga320x200:</b> Experimental video mode for low RAM consumption</li>
 <li><b>use_lib_log_serial:</b> Logs are sent by serial port usb</li>
</ul>


<br>
<Brightness mode</h1>
The brightness mode corresponds to 16 colors, so the 6-pin DAC output must be used. This mode is incompatible with the 8 colors mode, so if you choose this option, you have to disable the <b>use_lib_vga8colors</b> in the gbConfig.h.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewFantasy.gif'></center>
This mode consumes more CPU and RAM than the normal 8-color mode.


<br>
<Test applications
When starting the ESP32, it is done in ZX 48K mode, that is, loading the rom of the ZX 48k with the BASIC.
Every time we select a ROM, the tape selection is reset to block 0, as well as the sound.
Later we can choose the roms:
<ul>
 <li>
  <b>ROMS of 48K</b>
  <ul>
   <li>>DIAG:</b> diagnostic utility, for keyboard, ram, rom, 48k, 128k, AY8912 mode, speaker and video test.
   <li><b>SE:</b> Open SE rom BASIC, which allows you to use BASIC without the 48K mode function keys, so that each keystroke does not generate a function.</li>
   <li><b>SINCLAIR:</b> The 48k</li> mode 
  </ul>
 </li>
 <li>
  <b>ROMS of 128K</b>
  <ul>
   <li>>PLUS2A:</b> The spectrum +2</li>
   <li>>PLUS3:</b> The spectrum +3</li>
   <li>>PLUS3E:</b> The spectrum +3E</li>
   <li>>SINCLAIR:</b> The 128K</li>
  </ul>
 </li> 
</ul>
<br>
In the case of the SNA:
<ul> 
 <li>>DIAG:</b> ROM and RAM diagnostic utility.</li>
 <li><b>Fantasy: </b>Gloss support game (16 colors) It allows kempston, which together with the mouse, can be played very comfortably. 
 <li>>Sppong: </b> The classic pong</li>
 <Fire: It is a modified version of Fire, to support kempston mouse, so if you play without it, it will give problems. You can go to the main menu by pressing shift and the space bar. It has support for AY8912, however, the speaker output at the same time as AY8912 is not yet 100% effective. Separately, it is correct.
 <li>>3DcoolKm:</b> 3D tool with kempston mouse support</li>
 <li>>AT4openKM: </b> Drawing tool with kempston mouse support</li>
 <li><b>Sipka: </b> Kempston mouse test tool Allows to change the mouse icon by clicking on the buttons.</li>
 <MouseTestZXds: Tool for testing the kempston mouse protocol 
</ul>
<br>
For SCR loading from TAP:
<ul>
 <It doesn't load anything, because this tape only has the BASIC code of 3D. 
 <li><b>4x4:</b> Load the game car 4x4</li> 
</ul>
<br>
For SCRs:
<ul>
 <Load the image of the game 4x4. 
</ul>
<br>
For loading tapes from BASIC or Tape Loader:
<ul>
 <3D: Loads a 3D graphics simulation.
</ul>
