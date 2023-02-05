# Tiny ESP32 ZX Spectrum
Fork (October 2020) of David Crespo's emulator with the support of WII <a href='https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote'>Wiimote</a>, based on the emulator of Ramon Martinez and Jorge Fuertes <a href='https://github.com/rampa069/ZX-ESPectrum'>ZX-ESPectrum</a>, with dependencies of the author Pete Todd.
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
 <li>WIFI support load SCR and SNA</li>
 <li>Precompiled version (flash download 3.9.2) 320x200 core LKF,JLS and AY8912 support</li>
 <li>Option to remove the IEEE (double) rounding calculation from the VGA, to avoid accuracy issues. This anomaly was found as a result of David Crespo Tascón's failure to take video.</li>
 <li>Added 2 video modes 320x200 70Hz and 320x240 with fabgl data.</li>
 <li>Added keyboard support from usb serial terminal, VStudio monitor or putty.</li>
 <li>Added support for reading SNA (only 48K) and SCR from usb serial terminal, VStudio monitor or putty.</li>
</ul>

<br><br> 
<h1>Precompiled version</h1>
In the precompile folder there are 4 versions already compiled to be recorded with the flash download tool 3.9.2. They are the permutations of 2 cores (LKF and JLS) and AY8912 sound with 320x200 resolution with the demo games in FLASH.  A 320x200 option without IEEE calculation has also been added, as well as a 320x200 70 Hz video mode with fabgl parameters.<br><br>
<a href='https://github.com/rpsubc8/ESP32TinyZXSpectrum/tree/main/esp32/precompile'>https://github.com/rpsubc8/ESP32TinyZXSpectrum/tree/main/esp32/precompile</a>
<br><br>
We must choose the ESP32 type:
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/flash00.gif'></center>
Subsequently, we will select the files as shown in the attached screenshot, with the same offset values, which will be different according to the 4 options:
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/flash01.gif'></center>
And we will press start. If everything has been correct, we will only have to restart the ESP32.

<br><br>
<h1>Requirements</h1>
Required:
 <ul>
  <li>TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)</li>
  <li>Visual Studio 1.66.1 PLATFORMIO 2.4.3 Espressif32 v3.5.0 (python 3.6)</li>
  <li>Arduino IDE 1.8.11 Espressif System 1.0.6</li>
  <li>Arduino fabgl 0.9.0 reduced library (included in PLATFORMIO project)</li>
  <li>Arduino bitluni 0.3.3 reduced library (included in project)</li>
 </ul>
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/ttgovga32v12.jpg'></center>


<br><br>
<h1>PlatformIO</h1>
PLATFORMIO 2.4.3 must be installed from the Visual Studio extensions. Espressif32 v3.5.0 (python 3.6) is also required.
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
</li>
 </ul>
 There is a basic OSD of low resources, that is to say, very simple, that is visualized by pressing the key <b>F1</b>.
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewOSD.gif'></center>
 Files must be converted to .h in hexadecimal. You can use the online tool:<br>
 <a href='http://tomeko.net/online_tools/file_to_hex.php?lang=en'>http://tomeko.net/online_tools/file_to_hex.php?lang=en</a>
 

<br><br>
<h1>UART keyboard</h1>
If the option <b>use_lib_keyboard_uart</b> is activated, it allows to use the PC keyboard from the VStudio monitor or from the putty (115200 bauds), in a simple way, since it is not by reading SCANCODE down, up: 
<ul>
 <li><b>TAB key or F2 key:</b> Display OSD</li>
 <li><b>SHIFT+2 ("):</b> Send CONTROL + P to ZX48K</li>
 <li><b>BackSpace (delete):</b> Send SHIFT + 0 to the ZX48K</li>
 <li><b>ENTER:</b> Send ENTER on the ZX48K</li>
 <li><b>+:</b> Send ALT_GR</li>
 <li><b>-:</b> Send SHIFT LEFT</li>
 <li><b>*:</b> Send CONTROL</li> 
 <li><b>Up:</b> SHIFT + 7 and Kempston Up</li>
 <li><b>Down:</b> SHIFT + 6 and Kempston Down</li>
 <li><b>Right:</b> Kempston right</li>
 <li><b>Left:</b> Kempston left</li>
 <li><b>A..Z, a..z: </b> a..z</li>
</ul>
We can do key combinations at exactly the same time, such as * and P, so it would be like pressing CONTROL and P on the zx48K, which is equivalent to displaying ". From the Arduino IDE, this functionality is not allowed, since the serial monitor requires sending the ENTER for each action. 


<br><br>
<h1>SNA and SCR UART</h1>
Enabling the <b>use_lib_scr_uart</b> option will allow SCR files (6912 bytes) to be read from the screen by the terminal. Likewise, if the option <b>use_lib_sna_uart</b> is activated, it will allow to read 48K snapshots (49179 bytes), for now.<br>
If this option is activated, when we want to load an SCR or an SNA, the OSC menu will ask us if we want to use UART or Flash. If we choose UART, a message will appear on the screen waiting for data from the UART, and if it exceeds a certain timeout, it will stop reading. As soon as we send the data, it will start to display the loaded bytes.<br>
The way to send the data is by converting the binary file to ASCII hexadecimal format, without 0x, commas or strange characters. An example for a valid SCR would be:
<pre>
0000000000003000000038070000E001
C1C03800070E0E0E0E00383800E0E387

02020202020202020202020202020202
02020202020202020202020202020202
</pre>
As you can see, a byte contains 2 hexadecimal nibbles, for example the value of a 193 byte would be C1.<br>
It is normal to compose each line with 16 bytes, i.e. 32 nibbles. Each line will contain a carriage return. This conversion can be done conveniently with the utility:

<a href='https://tomeko.net/online_tools/file_to_hex.php?lang=en'>https://tomeko.net/online_tools/file_to_hex.php?lang=en</a>

We must uncheck the option to include 0x and comma separator, and we can leave the option to include new lines.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/tooltomeko.gif'></center>

An SCR (6912) in this format would occupy about 14688 bytes, which can be copied and pasted into the terminal. A 48K SNA (49179 bytes) in this format typically occupies 104504 bytes.<br>
If we use <b>putty</b>, we must choose the Serial connection with 115200 baud:
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/puttyconfig.gif'></center>
And once we see that everything is OK, we copy and paste the data.


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
 <li><b>use_lib_vga360x200x70hz_bitluni:</b> Normal video mode with bitluni parameters.</li>
 <li><b>use_lib_vga320x200x70hz_bitluni:</b> 320x200 video mode with bitluni parameters.</li> 
 <li><b>use_lib_vga320x240x60hz_bitluni:</b> 320x240 mode with bitluni parameters.</li>
 <li><b>use_lib_vga320x200x70hz_fabgl:</b>320x200 mode with fabgl parameters.</li>
 <li><b>use_lib_vga320x240x60hz_fabgl:</b>320x240 mode with fabgl settings.</li>
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
 <li><b>use_lib_wifi: </b> WIFI mode support for loading SCR's and SNA's. (48K mode only)</li>
 <li><b>use_lib_only_48k: </b> Only supports 48K mode, rom0, ram0, ram2 and ram5.</li> 
 <li><b>use_lib_fix_double_precision: </b> It doesn't use the VGA video frequency calculation with the ESP32 itself, avoiding possible accuracy problems with the mantissa. This is useful for ESP32's that miscalculate the frequency.</li>
 <li><b>use_lib_scr_uart</b> Allows loading SCR (6912 bytes) by VStudio terminal monitor or from putty</li>
 <li><b>use_lib_sna_uart</b> Allows to load SNA 48K (49179 bytes) via VStudio terminal monitor or from putty</li>
</ul>


<br><br>
<h1>Brightness mode</h1>
The brightness mode corresponds to 16 colors, so the 6-pin DAC output must be used. This mode is incompatible with the 8 colors mode, so if you choose this option, you have to disable the <b>use_lib_vga8colors</b> in the gbConfig.h.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewFantasy.gif'></center>
This mode consumes more CPU and RAM than the normal 8-color mode.


<br><br>
<h1>Test applications</h1>
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
<h1>WIFI support</h1>
A basic WIFI support has been added for TEST, to be able to load the SCR's and SNA's from a basic HTML server, without the need of CORS, so the deployment is very fast. Apache Server, NGINX, etc... can be used.<br>
By default, it has been left pointing to the project's github pages server:
<pre>
https://rpsubc8.github.io/ESP32TinyZXSpectrum/www/zxspectrum/output
</pre>

 To activate this mode, uncomment the line <b>use_lib_wifi</b> in <b>gbConfig.h</b><br>
Given the sram consumption, only 48K mode is allowed, when using WIFI.<br>
 We must configure in the <b>gbWIFIConfig.h</b> file the data:
 <pre>
  #define gb_wifi_ssd "nombreDeNuestraRedWIFIdelRooter"
  #define gb_wifi_pass "passwordDeNuestraRedWIFIdelRooter"

  //#define gb_wifi_url_base_path "http://192.168.0.36/zxspectrum/output"
  #define gb_wifi_url_base_path "https://rpsubc8.github.io/ESP32TinyZXSpectrum/www/zxspectrum/output"

  //millisecons delay stream read
  #define gb_wifi_delay_available 0

  #define use_lib_wifi_debug
 </pre>
 
 For now, the configuration is fixed in our <b>gb>gbWIFIConfig.h</b> that we will have to recompile, so, that it will only connect to our rooter network. Therefore, we must change <b>gb_wifi_ssd</b> and <b>gb_wifi_pass</b>.<br>
 The <b>gb_wifi_url_base_path</b> is the path where our <b>outlist</b> and <b>outdat</b> directories are located, which contain the list of files, as well as the files themselves, so this path will be different if we use a local server.<br><br>
 The concept is simple, you have:
 <pre>
  outlist --> File with the list of names (length 8) of SCR or SNA. Limit of 128 files
  outdat  --> SCR or SNA files.
 </pre>
 For now, to optimize RAM consumption, we have left an 8:3 name length structure, i.e. 8 name characters and 3 extension characters. I leave some intermediate tools to prepare and make the conversion:<br>
 <pre>
  build.bat --> Launches all bats, processing input to output
  
  data83.bat --> Converts all input files to 8:3 format
  
  list.bat --> Generates the outlist (list of files).
  dsk.exe --> Generates a txt file containing a list of files with name length 8.
  lowerscr.bat --> Convert SCR extensions to .scr
  lowersna.bat --> The same, but for SNA.
 </pre>

 An example of <b>outlist</b>, for example from action.txt, containing:
 <pre>
 amc1    batman  goody   robocop ThePunk 
 </pre>
 
 Whenever a file is added, we must regenerate the list with the <b>list.bat</b> or by calling the whole <b>build.bat</b> process.<br>
 
 Inside is the list of files with a maximum length of 8 characters, which is the one that will be shown in the SCR or SNA selection menu in the ESP32. These files, for now, are intended for a maximum of 128 entries, which is equivalent to 1024 bytes (128 x 8).<br>
 Each time a request is made to a type, the .TXT file with the list (1024 bytes, 128 names) is loaded. And when it is selected, the request is made to the file in the outdat.<br>
 When a file is selected, it will be loaded in <b>outdat</b> with its path. The files must have the extension in lower case.<br>
 
 If you are using an external WEB server, it is possible that policies may prevent you from making consecutive requests, so it is advisable not to make requests too close together.<br>
 
 To debug the WIFI, uncomment <b>use_lib_wifi_debug</b> in the <b>gbWifiConfig.h</b> file.
 
 

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
