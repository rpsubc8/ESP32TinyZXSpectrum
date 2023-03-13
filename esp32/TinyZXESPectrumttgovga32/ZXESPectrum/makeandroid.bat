@echo off
echo Convert ANDROID PROJECT DATA (WIN32)
echo Delete dataFlash (Borrar dataFlash)
pause

echo Process gbrom.h (Procesar gbrom.h)
fart dataFlash\gbrom.h "roms/rom" "rom"
ping -n 2 127.0.0.1>nul

echo Process gbscr.h (Procesar gbscr.h)
fart dataFlash\gbscr.h "scr/scr" "scr"
ping -n 2 127.0.0.1>nul

echo Process gbsna.h (Procesar gbsna.h)
fart dataFlash\gbsna.h "sna/sna" "sna"
ping -n 2 127.0.0.1>nul

echo Process gbtape.h (Procesar gbtape.h)
fart dataFlash\gbtape.h "tape/tape" "tape"
ping -n 2 127.0.0.1>nul



echo Process ZXESPectrum.ino gbrom (Procesar ZXESPectrum.ino)
fart ZXESPectrum.ino "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul

echo Process ZXESPectrum.ino gbscr (Procesar ZXESPectrum.ino)
fart ZXESPectrum.ino "dataFlash/gbscr.h" "gbscr.h"
ping -n 2 127.0.0.1>nul

echo Process ZXESPectrum.ino gbsna (Procesar ZXESPectrum.ino)
fart ZXESPectrum.ino "dataFlash/gbsna.h" "gbsna.h"
ping -n 2 127.0.0.1>nul

echo Process ZXESPectrum.ino gbtape (Procesar ZXESPectrum.ino)
fart ZXESPectrum.ino "dataFlash/gbtape.h" "gbtape.h"
ping -n 2 127.0.0.1>nul




echo Copy gbrom.h (Copiar gbrom.h)
copy/Y dataFlash\gbrom.h
ping -n 2 127.0.0.1>nul

echo Copy gbscr.h (Copiar gbscr.h)
copy/Y dataFlash\gbscr.h
ping -n 2 127.0.0.1>nul

echo Copy gbsna.h (Copiar gbsna.h)
copy/Y dataFlash\gbsna.h
ping -n 2 127.0.0.1>nul

echo Copy gbtape.h (Copiar gbtape.h)
copy/Y dataFlash\gbtape.h
ping -n 2 127.0.0.1>nul



echo Copy roms (Copiar roms)
copy/Y dataFlash\roms\*.h
ping -n 2 127.0.0.1>nul

echo Copy scr (Copiar scr)
copy/Y dataFlash\scr\*.h
ping -n 2 127.0.0.1>nul

echo Copy sna (Copiar sna)
copy/Y dataFlash\sna\*.h
ping -n 2 127.0.0.1>nul

echo Copy tape (Copiar tape)
copy/Y dataFlash\tape\*.h
ping -n 2 127.0.0.1>nul


echo Delete dataFlash (Borrar dataFlash)
rmdir /S /Q dataFlash

pause