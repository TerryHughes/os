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
    TokenType_Unknown,

    TokenType_PlusSign,
    TokenType_HyphenMinus,

    TokenType_Decimal,
    TokenType_Hexadecimal,

    TokenType_Label,
    TokenType_Times,
    TokenType_DefineByte,
    TokenType_Jump,

    TokenType_EndOfLine,
    TokenType_EndOfStream,
} AssemblerTokenType;

typedef struct
{
    u8 type;
    char *text;
    u8 length;
    u16 value;
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
    else if (at[result.length] == '+')
    {
        result.type = TokenType_PlusSign;
        result.length = 1;
    }
    else if (at[result.length] == '-')
    {
        result.type = TokenType_HyphenMinus;
        result.length = 1;
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
    else if (StringStartsWith(at, "jmp"))
    {
        result.type = TokenType_Jump;
        result.length = 3;
    }
    else if (StringStartsWith(at, "0x"))
    {
        result.type = TokenType_Hexadecimal;
        result.length = 2;

        while (1)
        {
            if (at[result.length] >= '0' && at[result.length] <= '9')
            {
                // TODO(TerryH): figure out a better way to multiply the value without having to specify it in each case
                result.value *= 16;
                result.value += at[result.length] - '0';
            }
            else if (at[result.length] >= 'A' && at[result.length] <= 'F')
            {
                result.value *= 16;
                result.value += at[result.length] - 'A' + 10;
            }
            else if (at[result.length] >= 'a' && at[result.length] <= 'f')
            {
                result.value *= 16;
                result.value += at[result.length] - 'a' + 10;
            }
            else
            {
                break;
            }

            ++result.length;
        }
    }
    else if ((at[result.length] >= 'A' && at[result.length] <= 'Z') ||
             (at[result.length] >= 'a' && at[result.length] <= 'z'))
    {
        result.type = TokenType_Label;

        while (at[result.length] != ':' && !StringStartsWith(at + result.length, "\r\n"))
        {
            ++result.length;
        }

        if (at[result.length] == ':')
        {
            ++result.length;
        }
    }
    else
    {
        result.type = TokenType_Decimal;

        while (1)
        {
            if (at[result.length] >= '0' && at[result.length] <= '9')
            {
                result.value *= 10;
                result.value += at[result.length] - '0';
            }
            else
            {
                break;
            }

            ++result.length;
        }
    }

    if (result.length == 0)
    {
        result.type = TokenType_Unknown;
    }

    return result;
}

void
Assemble(char *contents, u8 *output)
{
    u16 index = 0;

    Token label = {0};
    Token token = GetToken(contents);
    for (u16 times = 1; token.type != TokenType_EndOfStream; token = GetToken(token.text + token.length))
    {
        if (token.type == TokenType_Label)
        {
            label = token;
            label.value = index;
        }

        if (token.type == TokenType_Times)
        {
            token = GetToken(token.text + token.length);
            if (token.type == TokenType_Decimal ||
                token.type == TokenType_Hexadecimal)
            {
                times = token.value;
            }

            for (token = GetToken(token.text + token.length); token.type != TokenType_DefineByte; token = GetToken(token.text + token.length))
            {
                b32 addOperation = token.type == TokenType_PlusSign ? 1 : 0;
                token = GetToken(token.text + token.length);
                if (token.type == TokenType_Decimal ||
                    token.type == TokenType_Hexadecimal)
                {
                    times = addOperation ? times + token.value : times - token.value;
                }
            }
        }

        if (token.type == TokenType_DefineByte)
        {
            u8 value = 0;

            token = GetToken(token.text + token.length);
            if (token.type == TokenType_Decimal ||
                token.type == TokenType_Hexadecimal)
            {
                value = (u8)token.value;
            }

            for (; times > 0; --times)
            {
                output[index++] = value;
            }
            times = 1;
        }

        if (token.type == TokenType_Jump)
        {
            output[index++] = 0xEB;
            token = GetToken(token.text + token.length);
            if (token.type == TokenType_Decimal ||
                token.type == TokenType_Hexadecimal)
            {
                output[index++] = (u8)token.value;
            }
            else if (token.type == TokenType_Label)
            {
                output[index++] = (u8)(label.value - (index + 1));
            }
        }

        if (token.type == TokenType_Unknown)
        {
            break;
        }
    }
}
