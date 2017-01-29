#include <stdio.h>

#define Print(...) printf(## __VA_ARGS__)
#define PrintLine(...) Print(## __VA_ARGS__); Print("\n")

#if DEBUG
    #define DebugPrint(...) Print(## __VA_ARGS__)
    #define DebugPrintLine(...) PrintLine(## __VA_ARGS__)
#else
    #define DebugPrint(...)
    #define DebugPrintLine(...)
#endif
