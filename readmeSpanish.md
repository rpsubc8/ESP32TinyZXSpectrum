# Tiny ESP32 ZX Spectrum
Fork del emulador de David Crespo con el soporte de mando de WII <a href='https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote'>Wiimote</a>, basado a su vez, en el emulador de Ramon Martinez y Jorge Fuertes <a href='https://github.com/rampa069/ZX-ESPectrum'>ZX-ESPectrum</a>, con dependencias del autor Pete Todd.
<br>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewZx128k.gif'></center>
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
 <li>Emulación de AY8912 con fabgl</li>
 <li>Redirección de los pulsos de grabación de cinta al altavoz</li>
 <li>Opción de resampleo en modo speaker y mezcla con AY8912 (inestable)</li>
 <li>Posibilidad de elegir modo brillo y modos de 8 colores en compilación.</li>
 <li>Remapeo de los cursores en el menú de +2,+3, así como la tecla de borrar</li>
 <li>Ajuste de pantalla X, Y</li>
 <li>Escala de colores R, G, B, al estilo de las pantallas de fósforo verde</li>
 <li>Menú de velocidad de video de emulación</li>
 <li>Menú de velocidad de emulación por división de tiempo (5 por defecto).</li>
 <li>Soporte para leer archivos SCR</li>
 <li>Soporte para cargar archivos screen dentro de ficheros .TAP</li>
 <li>Carga de BASIC desde .TAP (intercepción de rutina de cinta)</li> 
 <li>Creado port de Windows bajo SDL</li>
 <li>Modos de video 360x200, 320x240, 320x200</li>  
</ul>

<br>
<h1>Requerimientos</h1>
Se requiere:
 <ul>
  <li>Visual Studio 1.48.1 PLATFORMIO 2.2.0</li>
  <li>Arduino IDE 1.8.11</li>
  <li>Librería Arduino fabgl 0.9.0 (incluida en proyecto PLATFORMIO). El el Arduino IDE se tiene que instalar.</li>
  <li>Librería Arduino bitluni 0.3.3 (incluida en proyecto)</li>
 </ul>
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/ttgovga32v12.jpg'></center>
<br>
 

<h1>PlatformIO</h1>
Se debe instalar el PLATFORMIO 2.2.0 desde las extensiones del Visual Studio.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIOinstall.gif'></center>
Luego se seleccionará el directorio de trabajo <b>TinyZXESPectrumttgovga32</b>.
Debemos modificar el fichero <b>platformio.ini</b> la opción <b>upload_port</b> para seleccionar el puerto COM donde tenemos nuestra placa TTGO VGA32.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIO.gif'></center>
Luego procederemos a compilar y subir a la placa. No se usa particiones, así que debemos subir todo el binario compilado.
Está todo preparado para no tener que instalar las librerias de bitluni. Sin embargo, si necesitamos las opciones de sonido y ratón, necesitaremos la librería <b>fabgl</b>, que se encuentra dentro del proyecto en PLATFORMIO.


<br>
<h1>Arduino IDE</h1>
Todo el proyecto es compatible con la estructura de Arduino 1.8.11.
Tan sólo tenemos que abrir el <b>ZX-ESPectrum.ino</b> del directorio <b>ZX-ESPectrum</b>.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewArduinoIDEpreferences.gif'></center>
Debemos instalar las extensiones de spressif en el gestor de urls adicionales de tarjetas <b>https://dl.espressif.com/dl/package_esp32_index.json</b>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewArduinoIDElibrary.gif'></center>
Para el modo normal, ya está preparado el proyecto, de forma que no se necesita ninguna librería de bitluni. Sin embargo, si necesitamos el soporte de ratón y sonido, se requiere instalar las librerias <b>fabgl 0.9.0</b>.
Lo podemos hacer desde el administrador de bibliotecas.
Debemos desactivar la opción de PSRAM, y en caso de superar 1 MB de binario, seleccionar 4 MB de partición a la hora de subir. Aunque el código no use PSRAM, si la opción está activa y nuestro ESP32 no dispone de ella, se generará una excepción y reinicio del mismo en modo bucle.


<br>
<h1>Usabilidad</h1>
Se permite cargar:
 <ul>
  <li>ROMS de 48K y 128K</li>
  <li>SNA de 48K</li>
  <li>Leer pantallas de ficheros .SCR. El fichero .SCR convertido a hexadecimal, contiene una imagen, la cual se vuelca en la zona de memoria de video. Los ficheros .SCR son muy fáciles de conseguir, si usamos el emulador <b>fuse</b>.</li>
  <li>Leer pantallas de cintas .TAP. Debemos tener un fichero .TAP convertido en hexadecimal con el formato básico de cargador BASIC, seguido de un bloque de datos de SCREEN. La rutina que he realizado es muy básica, y se encarga de buscar el bloque de pantalla, y volcarlo a memoria de video</li>
  <li>Seleccionar TAP permite elegir el .TAP y situarse en el bloque 0, para poder leer el BASIC de una cinta. De esta forma, si seleccionamos el <b>Tape loader</b> desde el menú del 128K,o si escribimos el comando <b>load ""</b> desde el BASIC, se procederá a cargar el código de BASIC. Debemos tener un fichero .TAP convertido en hexadecimal con el formato básico de cargador y código BASIC.</li>
 </ul>
 Se dispone de un OSD básico de bajos recursos, es decir, muy simple, que se visualiza pulsando la tecla <b>F1</b>.
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewOSD.gif'></center>
 Los ficheros deben ser convertidos a .h en hexadecimal. Puede usarse la herramienta online:<br>
 <a href='http://tomeko.net/online_tools/file_to_hex.php?lang=en'>http://tomeko.net/online_tools/file_to_hex.php?lang=en</a>
 
 
<br>
<h1>Opciones</h1>
El archivo <b>gbConfig.h</b> se seleccionan las opciones:
<ul>
 <li><b>use_lib_mouse_kempston:</b> Se usa un ratón conectado al puerto PS/2 emulando el protocolo kempston. Se utiliza la libreria fabgl 0.9.0</li>
 <li><b>use_lib_sound_ay8912:</b> Se utiliza un mezclador de 3 canales en modo dirty, emulando el AY8912. El speaker utiliza el canal 1 en modo digital, salvo que se active la opción de resampleo. Se requiere la librería fabgl 0.9.0</li>
 <li><b>use_lib_resample_speaker:</b> La salida del speaker se emula como onda senoidal en el canal 1. Está aún en un estado muy experimental. Se requiere la librería fabgl 0.9.0</li>
 <li><b>use_lib_redirect_tapes_speaker:</b> Redirige la salida de tonos de la cinta al altavoz.</li>
 <li><b>use_lib_vga8colors:</b> Obliga a usar RGB, sólo 3 pines y sin modo de brillo, 8 colores</li>
 <li><b>use_lib_vga64colors:</b> Obliga a usar RRGGBB, 6 pines, sin modo de brillo, es decir con 8 colores</li>
 <li><b>use_lib_use_bright:</b> Obliga a usar RRGGBB, 6 pines, con modo brillo, 16 colores</li>
 <li><b>use_lib_vga_low_memory:</b> Modo experimental de bajo consumo de RAM de video. En algún modo de video da problemas.</li>
 <li><b>use_lib_ultrafast_vga:</b> Modo rápido (el doble) de acceso a video. Puede dar problemas si se combina con el modo de video de bajo consumo de ram.</li> 
 <li><b>use_lib_vga_thread:</b> Utiliza la salida de video en un hilo. Si se hace en modo polling, no se usará ni el ajuste de pantalla, ni los modos de brillo. El modo polling está pensado para depuración</li>
 <li><b>use_lib_screen_offset:</b> Permite mover a la izquierda y arriba la pantalla.</li>
 <li><b>use_lib_skip_frame:</b> Permite saltarse frames al emular</li>
 <li><b>use_lib_vga360x200:</b> Modo de video normal</li>
 <li><b>use_lib_vga320x200:</b> Modo experimental de video para bajo consumo de RAM</li>
 <li><b>use_lib_vga320x240:</b> Modo experimental de video para bajo consumo de RAM</li>
 <li><b>use_lib_log_serial:</b> Se envian logs por puerto serie usb</li>
 <li><b>gb_ms_keyboard:</b> Se debe especificar el número de milisegundos de polling para el teclado.</li>
 <li><b>gb_ms_sound:</b> Se debe especificar el número de milisegundos de polling para el sonido AY8912.</li>
 <li><b>gb_frame_crt_skip:</b> El número de frames de video que deben saltarse.</li>
 <li><b>gb_delay_emulate_ms:</b> Milisegundos de espera por cada frame completado.</li>
 <li><b>gb_delay_emulate_div_microsec:</b> Se especifica la división de espera de tiempo en microsegundos de emulación (por defecto 5). A valor más alto, más rápido va la emulación. El valor 0, elimina la espera, es decir, es lo más rápido.</li> 
 <li><b>use_lib_tape_rom_intercept:</b> Controla la rutina 0x056B de leer cintas.</li>
 <li><b>use_lib_ultrafast_speaker:</b> Modo rápido de activación de pin E/S audio.</li>
</ul>


<br>
<h1>Modo brillo</h1>
El modo brillo, se corresponde con 16 colores, por tanto, se tiene que usar la salida del DAC de 6 pines. Este modo es incompatible con el modo de 8 colores, por tanto, si se elige esta opción, se tiene que deshabilitar el <b>use_lib_vga8colors</b> en el gbConfig.h.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewFantasy.gif'></center>
Este modo consume más CPU y RAM que el modo normal de 8 colores.



<br>
<h1>Aplicaciones Test</h1>
Al arrancar el ESP32, se realiza en modo ZX 48K, es decir, cargando la rom del ZX 48k con el BASIC.
Cada vez que seleccionamos una ROM, se resetea la selección de cinta al bloque 0, así como el sonido.
Posteriormente podemos elegir las roms:
<ul>
 <li>
  <b>ROMS de 48K</b>
  <ul>
   <li><b>DIAG:</b> utilidad de diagnósticos, para test de teclado, ram, rom, modo 48k, 128k, AY8912, speaker y video.</li>
   <li><b>SE:</b> El BASIC de Open SE rom, que permite usar el BASIC sin las teclas de funciones del modo 48K, de forma que cada pulsación de tecla no genera una función.</li>
   <li><b>SINCLAIR:</b> El modo 48k</li> 
  </ul>
 </li>
 <li>
  <b>ROMS de 128K</b>
  <ul>
   <li><b>PLUS2A:</b> El spectrum +2</li>
   <li><b>PLUS3:</b> El spectrum +3</li>
   <li><b>PLUS3E:</b> El spectrum +3E</li>
   <li><b>SINCLAIR:</b> El 128K</li>
  </ul>
 </li> 
</ul>
<br>
Para el caso de los SNA:
<ul> 
 <li><b>DIAG:</b> Utilidad de diagnóstico de la ROM y RAM.</li>
 <li><b>Fantasy: </b>Juego con soporte de brillo (16 colores). Permite kempston, que junto con el ratón, se puede jugar muy cómodo.</li> 
 <li><b>Sppong: </b> El pong clásico</li>
 <li><b>Fire: </b> Es una versión modificada del Fire, para soportar ratón kempston, por lo que si se juega sin él, dará problemas, pareciendo que se mueve lento. Se puede ir al menú principal pulsando shift y la barra espaciadora. Tiene soporte para AY8912, sin embargo, la salida del speaker al mismo tiempo que el AY8912 aún no es 100% efectivo. Por separado, es correcto.</li>
 <li><b>3DcoolKm:</b> Herramienta de 3D con soporte de ratón kempston</li>
 <li><b>AT4openKM: </b> Herramienta de dibujo con soporte de ratón kempston</li>
 <li><b>Sipka: </b> Tool para test de ratón kempston. Permite cambiar el icono del ratón al pulsar sobre los botones.</li>
 <li><b>MouseTestZXds: </b> Tool para test del protocolo del ratón kempston.</li>
 <li><b>FireKey: </b> el mismo juego Fire pero usando el teclado en lugar del ratón. Tiene salida de sonido AY8912 (modo 128K) y altavoz interno (modo 48K).</li>
 <li><b>Tritone1: </b> Musicdisk que usa altavoz interno.</li>
</ul>
<br>
Para la carga de SCR desde TAP:
<ul>
 <li><b>3D:</b> No carga nada, porque esta cinta sólo tiene el código de BASIC de 3D.</li> 
 <li><b>4x4:</b> Carga el coche del juego 4x4</li> 
</ul>
<br>
Para los SCR:
<ul>
 <li><b>4x4:</b> Carga la imagen del juego 4x4.</li> 
</ul>
<br>
Para la carga de cintas desde BASIC o Tape Loader:
<ul>
 <li><b>3D:</b> Carga una simulación de gráficas 3D.
</ul>


<br><br>
<h1>Tool data2h</h1>
He creado una herramienta muy básica, para convertir los archivos .rom, .tape, .sna, .scr en .h en modo lista para ser procesados por el emulador. Tan sólo tenemos que dejar los archivos en la carpeta <b>input</b> en cada tipo de carpeta y ejecutar el archivo <b>data.exe</b>, de forma que se generará una salida en el directorio <b>output/dataFlash</b>. Para pruebas, se han dejado varios archivos en cada directorio, que se recomienda borrar en caso de realizar una nueva lista personalizada.También se recomienda borrar los archivos del directorio <b>ESP32TinyZXSpectrum\dataFlash\</b> para tener un proyecto limpio.<br><br>
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
    nada.txt
</pre>
El concepto de archivos numéricos de rom (0.rom, 1.rom, 2.rom, 3.rom) sigue la misma base que el emulador del ZX-ESPectrum. El nombre de la carpeta es la que define el título de rom.
Es obligatorio dejar la carpeta <b>48k/sinclair/0.rom</b>, dado que necesita la rom del 48k para arrancar el emulador.
Posteriormente debemos copiar el directorio <b>dataFlash</b> en el proyecto <b>ESP32TinyZXSpectrum\ZX-ESPectrum</b> sobreescribiendo la carpeta dataFlash previa. Se recomienda limpiar el proyecto y volver a compilar.<br>
Esta herramienta es muy simple, y no controla los errores, por lo que se recomienda dejarle los archivos con nombres muy simples y lo más sencillo posible.<br>
El proyecto en PLATFORM.IO está preparado para 2 MB de Flash. Si necesitamos los 4MB de flash, tendremos que modificar la entrada del archivo <b>platformio.ini</b>
<pre>board_build.partitions = huge_app.csv</pre>
En el Arduino IDE, debemos elegir la opción <b>Partition Scheme (Huge APP)</b>.


<br><br>
<h1>Formato SNA</h1>
Está soportado el formato SNA de 48K (49179 bytes) y 128K (131103 bytes), compatible con los SNA del emulador FUSE.
El formato SNA de 48K:
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

El formato SNA de 128K:
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
El id del banco cacheado (bank n) se corresponde con los 3 bits menos significativos del puerto (0x7ffd & 0x07).
Después del byte 49183 del SNA, van los 8 bancos de 16384 bytes, eliminando el banco 5, 2 y el actualmente cacheado (banco n).
 
