: BLOCK_TEST :
: RUN_ASM :

#define VALUE 10
#undef VALUE
#define VALUE 42

start() {
    exit VALUE;
}

:/ OUTPUT
@exit_code=42
/:
