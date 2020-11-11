# Tiny ESP32 ZX Spectrum
Fork del emulador de David Crespo con el soporte de mando de WII <a href='https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote'>Wiimote</a>
Basado en el emulador de Ramon Martinez y Jorge Fuertes <a href='https://github.com/rampa069/ZX-ESPectrum'>ZX-ESPectrum</a>
<img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewZx128k.gif'>
<br>
He realizado varias modificaciones:
<ul>
 <li>No se usa PSRAM, funcionando en ESP32 de 520 KB de RAM (TTGO VGA32)</li> 
 <li>Soporta los modos 128K,+2,+3 y +3e sin falta de PSRAM</li>
 <li>Eliminado el soporte de mando WII</li>
 <li>Sustitución del OSD por uno de bajos recursos</li>
 <li>Creado proyecto compatible con Arduino IDE y Platform IO</li>
 <li>Eliminado el soporte de SPIFFS</li>
 <li>Recolocación de ROMs, SNA, .SCR, .TAP en Flash progmem</li>
 <li>Optimización de tamaño en FLASH</li> 
 <li>Optimización de la rutina de video</li>
 <li>Soporte para modo sin hilos</li> 
 <li>Emulación de ratón Kempston con fabgl</li>  
 <li>Emulación de AY8912</li>
 <li>Redirección de los pulsos de grabación de cinta al altavoz</li>
 <li>Opción de resampleo en modo speaker y mezcla con AY8912 (inestable)</li>
 <li>Posibilidad de elegir modo brillo y modos de 8 colores en compilación.</li>
 <li>Remapeo de los cursores en el menú de +2,+3, así como la tecla de borrar</li>
 <li>Ajuste de pantalla X, Y</li>
 <li>Escala de colores R, G, B, al estilo de las pantallas de fósforo verde</li>
 <li>Menú de velocidad de video de emulación</li>
 <li>Soporte para leer archivos SCR</li>
 <li>Soporte para cargar archivos screen dentro de ficheros .TAP</li>
 <li>Carga de BASIC desde .TAP (intercepción de rutina de cinta)</li> 
 <li>Creado port de Windows bajo SDL</li>  
</ul>


<h1>Opciones</h1>
Se requiere:
 <ul>
  <li>Visual Studio 1.48.1 PLATFORMIO</li>
  <li>Arduino IDE 1.8.11</li>
  <li>Library Arduino fabgl 0.9.0</li>
  <li>Arduino bitluni 0.3.3</li>
 </ul>
 

<h1>Opciones</h1>
El archivo <b>gbConfig.h</b> se seleccionan las opciones:
<ul>
 <li><b>use_lib_mouse_kempston:</b> Se usa un ratón conectado al puerto PS/2 emulado por portocolo kempston. Se utiliza la libreria fabgl 0.9.0</li>
 <li><b>use_lib_sound_ay8912:</b> Se utilizan un mezclador de 3 canales en modo dirty, emulando el AY8912. El speaker utiliza el canal 1 en modo digital, salvo que se active la opción de resampleo. Se requiere la librería fabgl 0.9.0</li>
 <li><b>use_lib_resample_speaker:</b> La salida del speaker se emula como onda senoidal en el canal 1. Se requiere la librería fabgl 0.9.0</li>
 <li><b>use_lib_redirect_tapes_speaker:</b> Redirige la salida de tonos de la cinta al altavoz.</li>
 <li><b>use_lib_vga8colors:</b> Obliga a usar RGB, sólo 3 pines y sin modo de brillo, 8 colores</li>
 <li><b>use_lib_use_bright:</b> Obliga a usar RRGGBB, 6 pines, con modo brillo, 16 colores</li>
 <li><b>use_lib_vga_low_memory:</b> Modo experimental de bajo consumo de RAM de video</li>
 <li><b>use_lib_vga_thread:</b> Utiliza la salida de video en un hilo. Si se hace ne modo polling, no se usará ni el ajuste de pantalla, ni los modos de brillo. El modo polling está pensado par depuración</li>
 <li><b>use_lib_screen_offset:</b> Permite mover a la izquierda y arriba la pantalla.</li>
 <li><b>use_lib_skip_frame:</b> Permite saltarse frames al emular</li>
 <li><b>use_lib_vga320x200:</b> Modo experimental de video para bajo consumo de RAM</li>
 <li><b>use_lib_log_serial:</b> Se envian logs por puerto serie usb</li>
</ul>
