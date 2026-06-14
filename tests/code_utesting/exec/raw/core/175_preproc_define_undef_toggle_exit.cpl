: BLOCK_TEST :
: RUN_ASM :

#define VALUE 9
#undef VALUE
#ifndef VALUE
#define VALUE 12
#endif

start() {
    exit VALUE as u8;
}

:/ OUTPUT
@exit_code=12
/:
