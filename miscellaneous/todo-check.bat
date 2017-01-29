@ECHO OFF
SETLOCAL

SET FileExtensions=*.h *.c *.cpp

ECHO TODOS FOUND:
findstr /L /S /I /N /C:"TODO" %FileExtensions%

ENDLOCAL
