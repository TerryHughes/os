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

b32
StringStartsWith(char *a, char *b)
{
    b32 result = a == b;

    if (!result && *a && *b)
    {
        while (*a == *b && *b)
        {
            ++a;
            ++b;
        }

        result = *b == 0;
    }

    return result;
}

typedef enum
{
    TokenType_Decimal,
    TokenType_Hexadecimal,

    TokenType_Times,
    TokenType_DefineByte,

    TokenType_EndOfLine,
    TokenType_EndOfStream,
} AssemblerTokenType;

typedef struct
{
    u8 type;
    char *text;
    u8 length;
} Token;

Token
GetToken(char *at)
{
    while (*at == ' ') ++at;

    Token result = {0};
    result.text = at;

    if (0)
    {
    }
    else if (!(*at))
    {
        result.type = TokenType_EndOfStream;
        result.length = 0;
    }
    else if (StringStartsWith(at, "\r\n"))
    {
        result.type = TokenType_EndOfLine;
        result.length = 2;
    }
    else if (StringStartsWith(at, "times"))
    {
        result.type = TokenType_Times;
        result.length = 5;
    }
    else if (StringStartsWith(at, "db"))
    {
        result.type = TokenType_DefineByte;
        result.length = 2;
    }
    else if (StringStartsWith(at, "0x"))
    {
        result.type = TokenType_Hexadecimal;
        result.length = 2;

        while ((at[result.length] >= '0' && at[result.length] <= '9') ||
               (at[result.length] >= 'A' && at[result.length] <= 'F') ||
               (at[result.length] >= 'a' && at[result.length] <= 'f'))
        {
            ++result.length;
        }
    }
    else
    {
        result.type = TokenType_Decimal;

        while (at[result.length] >= '0' && at[result.length] <= '9')
        {
            ++result.length;
        }
    }

    return result;
}

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

            u16 times = 1;
            Token token = GetToken((char *)readFileResult.contents);
            for (;token.type != TokenType_EndOfStream; token = GetToken(token.text + token.length))
            {
                if (token.type == TokenType_Times)
                {
                    times = 0;
                    token = GetToken(token.text + token.length);
                    for (u8 i = 0; i < token.length; ++i)
                    {
                        times *= 10;

                        times += token.text[i] - '0';
                    }
                }
                else if (token.type == TokenType_DefineByte)
                {
                    u8 value = 0;
                    token = GetToken(token.text + token.length);
                    if (token.type == TokenType_Hexadecimal)
                    {
                        for (u8 i = 2; i < token.length; ++i)
                        {
                            value *= 16;

                            if (token.text[i] >= '0' && token.text[i] <= '9')
                            {
                                value += token.text[i] - '0';
                            }
                            else if (token.text[i] >= 'A' && token.text[i] <= 'F')
                            {
                                value += token.text[i] - 'A' + 10;
                            }
                            else if (token.text[i] >= 'a' && token.text[i] <= 'f')
                            {
                                value += token.text[i] - 'a' + 10;
                            }
                        }
                    }
                    else
                    {
                        for (u8 i = 0; i < token.length; ++i)
                        {
                            value *= 10;

                            value += token.text[i] - '0';
                        }
                    }
                    for (;times > 0; --times)
                    {
                        output.memory[memoryIndex++] = value;
                    }
                    times = 1;
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
