: BLOCK_TEST :
: RUN_ASM :

#define OUTER411 1
#define VALUE411 2
#ifdef OUTER411
    #undef VALUE411
    #define VALUE411 40
    #ifndef INNER411
        #define EXTRA411 2
    #endif
#endif

start() {
    exit VALUE411 + EXTRA411;
}

:/ OUTPUT
@exit_code=42
/:
