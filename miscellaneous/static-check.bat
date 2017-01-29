@ECHO OFF
SETLOCAL

SET FileExtensions=*.h *.c *.cpp

ECHO STATICS FOUND:
findstr /L /S /I /N /C:"static" %FileExtensions%

ENDLOCAL
