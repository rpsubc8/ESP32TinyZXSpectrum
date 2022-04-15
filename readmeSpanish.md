# Tiny ESP32 ZX Spectrum
Fork del emulador de David Crespo con el soporte de mando de WII <a href='https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote'>Wiimote</a>, basado a su vez, en el emulador de Ramon Martinez y Jorge Fuertes <a href='https://github.com/rampa069/ZX-ESPectrum'>ZX-ESPectrum</a>, con dependencias del autor Pete Todd.
<br>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewZx128k.gif'></center>
He realizado varias modificaciones:
<ul>
 <li>No se usa PSRAM, funcionando en ESP32 de 520 KB de RAM (TTGO VGA32 v1.x)</li> 
 <li>Soporta los modos 128K,+2,+3 y +3e sin falta de PSRAM</li>
 <li>Eliminado el soporte de mando WII</li>
 <li>Sustitución del OSD por uno de bajos recursos</li>
 <li>Creado proyecto compatible con Arduino IDE y Platform IO</li>
 <li>Eliminado el soporte de SPIFFS</li>
 <li>Recolocación de ROMs, SNA, .SCR, .TAP en Flash progmem</li>
 <li>Optimización de tamaño en FLASH</li> 
 <li>Optimización de la rutina de video (2200 microsegundos por frame)</li>
 <li>Soporte para modo con (50 fps 20000 microsegundos) y sin hilos (50 fps 17000 microsegundos)</li>
 <li>Emulación de ratón Kempston (librería reducida de Rob Kent jazzycamel)</li>
 <li>Emulación Kempston con teclas cursores (izquierda, derecha, arriba, abajo) y ALT derecho.</li> 
 <li>Emulación de AY8912 con fabgl, versión reducida de dcrespo3d (David Crespo Tascón)</li>
 <li>Redirección de los pulsos de grabación de cinta al altavoz</li>
 <li>Opción de resampleo en modo speaker y mezcla con AY8912 (inestable)</li>
 <li>Posibilidad de elegir modo brillo y modos de 8 colores en compilación.</li>
 <li>DAC VGA 64 colores (6 bits) y 8 colores (3 bits) (versión reducida de Ricardo Massaro)</li>
 <li>Remapeo de los cursores en el menú de +2,+3, así como la tecla de borrar</li>
 <li>Ajuste de pantalla X, Y (consume un poco de CPU)</li>
 <li>Escala de colores R, G, B, al estilo de las pantallas de fósforo verde</li>
 <li>Menú de velocidad de video de emulación</li>
 <li>Menú de velocidad de emulación AUTO (50 fps 20000 microsegundos) así como un delay en microsegundos o sin freno, de 150 fps a 396 fps en el core de Lin Ke-Fong o desde los 95 fps a los 225 fps en el core de José Luis Sánchez.</li>
 <li>Soporte para leer archivos SCR</li>
 <li>Soporte para cargar archivos screen dentro de ficheros .TAP</li>
 <li>Carga de BASIC desde .TAP (intercepción de rutina de cinta). Sólo en el core de Lin Ke-Fong</li> 
 <li>Soporte de core Lin Ke-Fong y de José Luis Sánchez</li>
 <li>Creado port de Windows bajo SDL (no disponible para descargar)</li>
 <li>Modos de video 360x200, 320x240, 320x200</li> 
 <li>Soporte de SNA de 128K</li> 
</ul>

<br><br>
<h1>Requerimientos</h1>
Se requiere:
 <ul>
  <li>TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)</li>
  <li>Visual Studio 1.66.1 PLATFORMIO 2.4.3 Espressif32 v3.5.0 (python 3.6)</li>
  <li>Arduino IDE 1.8.11 Espressif System 1.0.6</li>
  <li>Librería reducida Arduino fabgl 0.9.0 (incluida en proyecto PLATFORMIO)</li>
  <li>Librería reducida Arduino bitluni 0.3.3 (incluida en proyecto)</li>
 </ul>
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/ttgovga32v12.jpg'></center>
<br><br>
 

<h1>PlatformIO</h1>
Se debe instalar el PLATFORMIO 2.2.1 desde las extensiones del Visual Studio. Se requiere también Espressif32 v3.3.2. 
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIOinstall.gif'></center>
Luego se seleccionará el directorio de trabajo <b>TinyZXESPectrumttgovga32</b>.
Debemos modificar el fichero <b>platformio.ini</b> la opción <b>upload_port</b> para seleccionar el puerto COM donde tenemos nuestra placa TTGO VGA32.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewPlatformIO.gif'></center>
Luego procederemos a compilar y subir a la placa. No se usa particiones, así que debemos subir todo el binario compilado. Está todo preparado para no tener que instalar las librerias de bitluni ni fabgl.

<br><br>
<h1>Arduino IDE</h1>
Todo el proyecto es compatible con la estructura de Arduino 1.8.11.
Tan sólo tenemos que abrir el <b>ZXESPectrum.ino</b> del directorio <b>ZXESPectrum</b>.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewArduinoIDEpreferences.gif'></center>
Debemos instalar las extensiones de spressif en el gestor de urls adicionales de tarjetas <b>https://dl.espressif.com/dl/package_esp32_index.json</b>
Ya está preparado el proyecto, de forma que no se necesita ninguna librería de bitluni ni fabgl. Debemos desactivar la opción de PSRAM, y en caso de superar 1 MB de binario, seleccionar 4 MB de partición a la hora de subir. Aunque el código no use PSRAM, si la opción está activa y nuestro ESP32 no dispone de ella, se generará una excepción y reinicio del mismo en modo bucle.


<br><br>
<h1>Usabilidad</h1>
Se permite cargar:
 <ul>
  <li>ROMS de 48K y 128K</li>
  <li>SNA de 48K</li>
  <li>Leer pantallas de ficheros .SCR. El fichero .SCR convertido a hexadecimal, contiene una imagen, la cual se vuelca en la zona de memoria de video. Los ficheros .SCR son muy fáciles de conseguir, si usamos el emulador <b>fuse</b>.</li>
  <li>Leer pantallas de cintas .TAP. Debemos tener un fichero .TAP convertido en hexadecimal con el formato básico de cargador BASIC, seguido de un bloque de datos de SCREEN. La rutina que he realizado es muy básica, y se encarga de buscar el bloque de pantalla, y volcarlo a memoria de video</li>
  <li>Seleccionar TAP permite elegir el .TAP y situarse en el bloque 0, para poder leer el BASIC de una cinta. De esta forma, si seleccionamos el <b>Tape loader</b> desde el menú del 128K,o si escribimos el comando <b>load ""</b> desde el BASIC, se procederá a cargar el código de BASIC. Debemos tener un fichero .TAP convertido en hexadecimal con el formato básico de cargador y código BASIC. Sólo está activo para el core de Lin Ke-Fong, y sólo carga el primer bloque de un programa BASIC. Si ese programa BASIC termina lanzando un RUN o un LOAD, no llegará a procesar esa nueva carga de datos, provocando un reinicio o algo aleatorio.</li>
 </ul>
 Se dispone de un OSD básico de bajos recursos, es decir, muy simple, que se visualiza pulsando la tecla <b>F1</b>.
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewOSD.gif'></center>
 Los ficheros deben ser convertidos a .h en hexadecimal. Puede usarse la herramienta online:<br>
 <a href='http://tomeko.net/online_tools/file_to_hex.php?lang=en'>http://tomeko.net/online_tools/file_to_hex.php?lang=en</a>
 
 
<br><br>
<h1>Opciones</h1>
El archivo <b>gbConfig.h</b> se seleccionan las opciones:
<ul>
 <li><b>use_lib_mouse_kempston:</b> Se usa un ratón conectado al puerto PS/2 emulando el protocolo kempston. Se utiliza la libreria reducida de Rob Kent. Al usar un modo sin interrupciones, se consume CPU en cada iteración, reduciendo frames de emulación.</li>
 <li><b>define use_lib_mouse_kempston </b> Si está activa, permite elegir por defecto el ratón en modo zurdo. En cualquier momento, desde el OSD,se puede también cambiar, así como elegir la inversión de la X e Y, y la velocidad de incremento.</li>
 <li><b>gb_delay_init_ms: </b> Permite especificar los milisegundos de espera por la inicialización del ratón.</li>
 <li><b>gb_ms_mouse: </b> Permite especificar los milisegundos de polling del ratón.</li>
 <li><b>use_lib_sound_ay8912:</b> Se utiliza un mezclador de 3 canales en modo dirty, emulando el AY8912. El speaker utiliza el canal 1 en modo digital, salvo que se active la opción de resampleo. Se requiere la librería fabgl reducida, que ya se encuentra dentro del proyecto</li> 
 <li><b>use_lib_resample_speaker:</b> La salida del speaker se emula como onda senoidal en el canal 1. Está aún en un estado muy experimental.</li>
 <li><b>use_lib_redirect_tapes_speaker:</b> Redirige la salida de tonos de la cinta al altavoz.</li>
 <li><b>use_lib_vga8colors:</b> Obliga a usar RGB, sólo 3 pines y sin modo de brillo, 8 colores</li>
 <li><b>use_lib_vga64colors:</b> Obliga a usar RRGGBB, 6 pines, sin modo de brillo, es decir con 8 colores</li>
 <li><b>use_lib_use_bright:</b> Obliga a usar RRGGBB, 6 pines, con modo brillo, 16 colores</li>
 <li><b>use_lib_vga_thread:</b> Utiliza la salida de video en un hilo. Si está comentada, se usará un sólo núcleo.</li>
 <li><b>use_lib_screen_offset:</b> Permite mover a la izquierda y arriba la pantalla. Consume un poco de CPU, un par de microsegundos. Si está activo, se puede ajustar desde el OSD. Lo ideal, es no tenerlo activo y hacer los ajustes desde el monitor.</li>
 <li><b>use_lib_skip_frame:</b> Permite saltarse frames al emular</li>
 <li><b>use_lib_vga360x200:</b> Modo de video normal</li>
 <li><b>use_lib_vga320x200:</b> Modo de vídeo 320x200</li>
 <li><b>use_lib_vga320x240:</b> Modo 320x240</li>
 <li><b>use_lib_log_serial:</b> Se envian logs por puerto serie usb</li>
 <li><b>gb_ms_keyboard:</b> Se debe especificar el número de milisegundos de polling para el teclado.</li>
 <li><b>gb_ms_sound:</b> Se debe especificar el número de milisegundos de polling para el sonido AY8912.</li>
 <li><b>gb_frame_crt_skip:</b> El número de frames de video que deben saltarse.</li>
 <li><b>gb_delay_emulate_ms:</b> Milisegundos de espera por cada frame completado.</li>
 <li><b>use_lib_tape_rom_intercept:</b> Controla la rutina 0x056B de leer cintas. Sólo está activo para el core de Lin Ke-Fong, y sólo carga el primer bloque de un programa BASIC. Consume unos ciclos de CPU, al estar chequeando el registro PC.</li>
 <li><b>use_lib_ultrafast_speaker:</b> Modo rápido de activación de pin E/S audio.</li>
 <li><b>use_lib_stats_time_unified: </b> Permite ver trazas por el puerto serie con los fps, así como microsegundos actuales, mínimos y máximos por frame. Lo mismo para el video.</li>
 <li><b>use_lib_delay_tick_cpu_auto: </b> Si vale 1, permite autoajustar la velocidad del frame a 20000 microsegundos, es decir 20 milisegundos, que equivale a 50 fps. Posteriormente desde el OSD se puede cambiar la velocidad de espera de la CPU.</li>
 <li><b>use_lib_delay_tick_cpu_micros: </b> Si es 0, no se aplica espera en cada ejecución de intrucción.</li>
 <li><b>use_lib_cpu_adjust_mask_instructions: </b> Aplica una mascara de reajuste de tiempo para los 20000 microsegundos de cada frame. Si la mascara es 0x03, se aplica la máscara binaria 00000011, es decir, cada 4 intrucciones.</li>
 <li><b>use_lib_core_linkefong: </b> Permite elegir el core de Lin Ke-Fong. Una vez compilado, aparecerá indicado en el OSD.</li>
 <li><b>use_lib_core_jsanchezv: </b> Permite elegir el core de José Luis Sánchez. Una vez compilado, aparecerá indicado en el OSD.</li>
 <li><b>use_lib_delayContention: </b> Si está activo, permite aplicar la espera de la memoria contenida, sólo en el core de José Luis Sánchez.</li>
 <li><b>use_lib_wifi: </b> Soporte de modo WIFI para cargar SCR's y SNA's. (sólo modo 48K)</li>
 <li><b>use_lib_only_48k: </b> Sólo soporta el modo 48K, rom0, ram0, ram2 y ram5</li>
</ul>


<br><br>
<h1>Modo brillo</h1>
El modo brillo, se corresponde con 16 colores, por tanto, se tiene que usar la salida del DAC de 6 pines. Este modo es incompatible con el modo de 8 colores, por tanto, si se elige esta opción, se tiene que deshabilitar el <b>use_lib_vga8colors</b> en el gbConfig.h.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyZXSpectrum/main/preview/previewFantasy.gif'></center>
Este modo consume más CPU y RAM que el modo normal de 8 colores.



<br><br>
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
 <li><b>Z80FULL: </b> El Test de z80 completo. Se debe ejecutar con el comando <b>RUN</b>.</li>
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
<h1>Soporte WIFI</h1>
Se ha añadido para TEST un soporte básico de WIFI, para poder cargar los SCR's y SNA's desde un servidor básico HTML, sin necesidad de CORS, por lo que el despliegue es muy rápido. Se puede usar un Servidor Apache, NGINX, etc...<br>
Por defecto, se ha dejado apuntando al servidor de pages de github del proyecto:
<pre>
https://rpsubc8.github.io/ESP32TinyZXSpectrum/www/zxspectrum/output
</pre>

 Para activar este modo, se debe descomentar la línea <b>use_lib_wifi</b> en el <b>gbConfig.h</b><br>
Dado el consumo de sram, sólo se permite el modo 48K, cuando se usa la WIFI.<br>
 Debemos configurar en el archivo <b>gbWIFIConfig.h</b> los datos:
 <pre>
  #define gb_wifi_ssd "nombreDeNuestraRedWIFIdelRooter"
  #define gb_wifi_pass "passwordDeNuestraRedWIFIdelRooter"

  //#define gb_wifi_url_base_path "http://192.168.0.36/zxspectrum/output"
  #define gb_wifi_url_base_path "https://rpsubc8.github.io/ESP32TinyZXSpectrum/www/zxspectrum/output"

  //millisecons delay stream read
  #define gb_wifi_delay_available 0

  #define use_lib_wifi_debug
 </pre>
 
 Por ahora, la configuración está fijada en nuestro <b>gbWIFIConfig.h</b> que tendremos que recompilar, de manera, que sólo se conectará a nuestra red del rooter. Por tanto debemos de cambiar <b>gb_wifi_ssd</b> y <b>gb_wifi_pass</b>.<br>
 El <b>gb_wifi_url_base_path</b> es la ruta en donde se encuentran nuestros directorios <b>outlist</b> y <b>outdat</b>, que contienen el listado de archivos, así como los mismos, por lo que dicho path será diferente si usamos un servidor local.<br><br>
 El concepto es simple, se dispone de:
 <pre>
  outlist --> Fichero con la lista de nombres (longitud 8) de SCR o SNA. Límite de 128 ficheros
  outdat  --> Los fichero SCR o SNA.
 </pre>
 Por ahora, para optimizar el consumo de RAM, se ha dejado una estructura de longitud de nombres 8:3, es decir, 8 caracteres de nombre y 3 de extensión. Dejo unas tools intermedias para preparar y hacer la conversión:<br>
 <pre>
  build.bat --> Lanza todos los bats, procesando input en output
  
  data83.bat --> Convierte todos los archivos input a formato 8:3
  
  list.bat --> Genera los outlist (lista de archivos).
  dsk.exe --> Genera un txt que dentro contiene la lista de archivos con longitud de nombre 8.
  lowerscr.bat --> Convierte las extensiones SCR a .scr
  lowersna.bat --> Lo mismo, pero para SNA.
 </pre>

 Un ejemplo de <b>outlist</b>, por ejemplo de action.txt, que contiene:
 <pre>
 amc1    batman  goody   robocop ThePunk 
 </pre>
 
 Siempre que se añada un fichero, debemos de regenerar la lista con el <b>list.bat</b> o bien llamando a todo el proceso <b>build.bat</b>
 
 Dentro esta la lista de archivos con longitud máxima de 8 caracteres, que es la que se mostrará en el menu de selección de SCR o SNA en el ESP32. Estos archivos, por ahora están pensados para un máximo de 128 entradas, que equivale a 1024 bytes (128 x 8).<br>
 Cada vez que se hace una petición a un tipo, se carga el fichero .TXT con la lista (1024 bytes, 128 nombres). Y cuando se selecciona, se hace la petición al fichero que se encuentra en el outdat.<br>
 Cuando se seleccione un archivo, se procederá a cargarlo en <b>outdat</b> con su ruta. Los archivos tienen que tener la extensión en minúsculas.<br>
 
 Si se usa un servidor externo WEB, es posible, que por políticas impida realizar peticiones seguidas, así que es recomendable no hacer peticiones muy seguidas.<br>
 
 Para depurar la WIFI, se debe descomentar <b>use_lib_wifi_debug</b> el el fichero <b>gbWifiConfig.h</b>
 
 

<br><br>
<h1>Formato SNA</h1>
Está soportado el formato SNA de 48K (49179 bytes) y 128K (131103 bytes), compatible con los SNA del emulador FUSE.<br>
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
 
