#include "..\\..\\..\\common\\types.h"
#include "..\\..\\..\\common\\macros.h"
#include "..\\..\\..\\common\\logging.h"
#include "..\\..\\..\\common\\functions.c"

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
            u16 memoryIndex = 0;

            u8 *at = readFileResult.contents;
            while (*at)
            {
                if (at[0] == 'd' && at[1] == 'b' && at[2] == ' ')
                {
                    at += 3;
                    u8 length = 0;
                    while (at[length] != '\r') ++length;

                    u8 value = 0;
                    if (at[0] == '0' && at[1] == 'x')
                    {
                        for (int i = 2; i < length; ++i)
                        {
                            value *= 16;

                            if (at[i] >= '0' && at[i] <= '9')
                            {
                                value += at[i] - '0';
                            }
                            else if (at[i] >= 'A' && at[i] <= 'F')
                            {
                                value += at[i] - 'A' + 10;
                            }
                            else if (at[i] >= 'a' && at[i] <= 'f')
                            {
                                value += at[i] - 'a' + 10;
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < length; ++i)
                        {
                            value *= 10;

                            value += at[i] - '0';
                        }
                    }
                    output.memory[memoryIndex++] = value;
                    at += length;
                }
                else if (at[0] == '\r' && at[1] == '\n')
                {
                    at += 2;
                }
                else
                {
                    ++at;
                }
            }

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
