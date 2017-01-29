#if !defined(internal)
    #define internal static
#endif

#define U64Maximum ((u64)-1)

#if SLOW
    #define Assert(expression) if (!(expression)) { *(int *)0 = 0; }
#else
    #define Assert(expression)
#endif
