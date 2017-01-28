@ECHO OFF
SETLOCAL

SET DisabledCompilerWarning=/wd4100
SET CommomCompilerFlags=/D WINDOWSAPI /Z7 /nologo %DisabledCompilerWarning% /W4 /WX
SET CommonLinkerFlags=/WX

IF "%1"=="release" GOTO release

:debug
SET CommonCompilerFlags=%CommomCompilerFlags% /D DEBUG
SET CommonCompilerFlags=%CommomCompilerFlags% /D SLOW
SET CommonLinkerFlags=%CommomLinkerFlags% /DEBUG
GOTO compile

:release
SET CommomCompilerFlags=%CommomCompilerFlags% /O2
GOTO compile

:compile
IF NOT EXIST data MKDIR data
IF NOT EXIST build MKDIR build

SET tool=assembler
ctime -begin data\%tool%.ctm
cl %CommomCompilerFlags% /Fe:build\%tool% /Fm:build\%tool% /Fo:build\%tool% toolchain\%tool%\platforms\WindowsAPI\main.c /link %CommonLinkerFlags%
ctime -end data\%tool%.ctm %ERRORLEVEL%

ctags --exclude=data --recurse *

ENDLOCAL
