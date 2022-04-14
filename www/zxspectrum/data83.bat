@echo off
rem remove prev directory files dat output
rmdir/S/Q output\outdat

rem copy input to output convert name 8:3
xcopy/N/E/Y/I input output\outdat

rem remove nada.txt empty
del/S output\outdat\nada.txt

rem lowercase convert extension SCR
rem ren output\outdat\scr\*.SCR *.scr
call lowerscr.bat

rem lowercase convert extension SNA
rem ren output\outdat\SNA\*.SNA *.sna
call lowersna.bat
