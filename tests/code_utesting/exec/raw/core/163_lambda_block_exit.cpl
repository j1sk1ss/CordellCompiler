: BLOCK_TEST :
: RUN_ASM :

start() {
    ptr i0 twice_plus = (i64 a, i64 b) => {
        i64 t = a * 2;
        return t + b;
    };

    exit twice_plus(8, 3) as u8;
}

:/ OUTPUT
@exit_code=19
/:
