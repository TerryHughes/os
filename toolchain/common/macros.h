#define internal static
#define global_variable static

#define U32Maximum ((u32)-1)

#if SLOW
    #define Assert(expression) if (!(expression)) { *(int *)0 = 0; }
#else
    #define Assert(expression)
#endif
