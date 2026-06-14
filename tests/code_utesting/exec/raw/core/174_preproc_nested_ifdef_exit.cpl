: BLOCK_TEST :
: RUN_ASM :

#define OUTER 1
#define INNER 1

start() {
    i32 x = 0;

#ifdef OUTER
#ifdef INNER
    x = 17;
#endif
#endif

    exit x as u8;
}

:/ OUTPUT
@exit_code=17
/:
