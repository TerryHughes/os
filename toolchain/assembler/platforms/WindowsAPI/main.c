#include "..\\..\\..\\common\\types.c"
#include "..\\..\\..\\common\\macros.c"
#include "..\\..\\..\\common\\logging.c"
#include "..\\..\\..\\common\\functions.c"

#include "..\\..\\..\\common\\platforms\\WindowsAPI\\file.h"
#include <windows.h>
#include "..\\..\\..\\common\\platforms\\WindowsAPI\\file.c"

int
main(int argc, char *argv[], char *envp[])
{
    int result = ReturnCode_Undetermined;

    if (argc < 2)
    {
        PrintLine("USAGE: assembler {input file}");

        result = ReturnCode_InvalidUsage;
    }
    else
    {
        char *filePath = argv[1];
        ReadFileResult readFileResult = WindowsAPI_ReadFile(filePath);
        result = readFileResult.status;

        if (readFileResult.status == FileOperationStatus_Success)
        {
            PrintLine("success\n%d\n%s", readFileResult.contentSize, (char *)readFileResult.contents);
        }
    }

    return result;
}
