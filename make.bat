@ECHO OFF
SETLOCAL

SET DisabledCompilerWarning=/wd4100
SET CommonCompilerFlags=/D WINDOWSAPI /Z7 /nologo %DisabledCompilerWarning% /W4 /WX
SET CommonLinkerFlags=/WX

IF "%1"=="release" GOTO release

:debug
SET CommonCompilerFlags=%CommonCompilerFlags% /D DEBUG
SET CommonCompilerFlags=%CommonCompilerFlags% /D SLOW
SET CommonLinkerFlags=%CommomLinkerFlags% /DEBUG
GOTO compile

:release
SET CommonCompilerFlags=%CommonCompilerFlags% /O2
GOTO compile

:compile
IF NOT EXIST data MKDIR data
IF NOT EXIST build MKDIR build

SET ctimeAvailable=false
WHERE ctime > NUL 2> NUL
IF %ERRORLEVEL%==0 SET ctimeAvailable=true

SET tool=assembler
IF %ctimeAvailable%==true ctime -begin data\%tool%.ctm
cl %CommonCompilerFlags% /Fe:build\%tool% /Fm:build\%tool% /Fo:build\%tool% toolchain\%tool%\platforms\WindowsAPI\main.c /link %CommonLinkerFlags%
IF %ctimeAvailable%==true ctime -end data\%tool%.ctm %ERRORLEVEL%

SET ctagsAvailable=false
WHERE ctags > NUL 2> NUL
IF %ERRORLEVEL%==0 SET ctagsAvailable=true

IF %ctagsAvailable%==true ctags --exclude=data --recurse *
