#include "..\\..\\..\\common\\types.h"
#include "..\\..\\..\\common\\macros.h"
#include "..\\..\\..\\common\\logging.h"
#include "..\\..\\..\\common\\functions.c"

#include "..\\..\\assembler.cpp"

typedef struct
{
    u32 memorySize;
    u8 *memory;
} Output;

#include "..\\..\\..\\common\\platforms\\WindowsAPI\\file.h"
#include <windows.h>
#include "..\\..\\..\\common\\platforms\\WindowsAPI\\file.c"

int
main(int argc, char *argv[], char *envp[])
{
    int result = ReturnCode_Undetermined;

    if (argc < 3)
    {
        PrintLine("USAGE: assembler {input file} {output file}");

        result = ReturnCode_InvalidUsage;
    }
    else
    {
        char *inputFilePath = argv[1];
        char *outputFilePath = argv[2];

        ReadFileResult readFileResult = WindowsAPI_ReadFile(inputFilePath);
        result = readFileResult.status;

        if (readFileResult.status == FileOperationStatus_Success)
        {
            PrintLine("success\n%d\n%s", readFileResult.contentSize, (char *)readFileResult.contents);

            Output output = {0};
            output.memorySize = 512;
            output.memory = VirtualAlloc(NULL, output.memorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            Assemble((char *)readFileResult.contents, output.memory);

            WriteFileResult writeFileResult = WindowsAPI_WriteFile(outputFilePath, output);
            result = writeFileResult.status;

            if (writeFileResult.status == FileOperationStatus_Success)
            {
                PrintLine("success");
            }
        }
    }

    return result;
}
