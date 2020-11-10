# Tiny ESP32 ZX Spectrum
Fork del emulador de David Crespo con el soporte de mando de WII:
<a href='https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote'>Wiimote</a>
Basado en el emulador de Ramon Martinez y Jorge Fuertes:
<a href='https://github.com/rampa069/ZX-ESPectrum'>ZX-ESPectrum</a>
He realizado varias modificaciones:
<ul>
 <li>No se usa PSRAM, funcionando en ESP32 de 520 KB de RAM (TTGO VGA32)</li> 
 <li>Soporta los modos 128K,+2,+3 y +3e sin falta de PSRAM</li>
 <li>Eliminado el soporte de mando WII</li>
 <li>Sustitución del OSD por uno de bajos recursos</li>
 <li>Creado proyecto compatible con Arduino IDE y Platform IO</li>
 <li>Eliminado el soporte de SPIFFS</li>
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
 <li>Menú de velocidad de video de emulación</li>
 <li>Soporte para leer archivos SCR</li>
 <li>Soporte para cargar archivos screen dentro de ficheros .TAP</li>
 <li>Carga de BASIC desde .TAP (intercepción de rutina de cinta)</li> 
 <li>Creado port de Windows bajo SDL</li>  
</ul>
