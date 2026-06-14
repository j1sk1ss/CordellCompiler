: BLOCK_TEST :
: RUN_ASM :

start() {
    ptr i8 s = ref "a1b2c3";
    i32 digits = 0;

    while dref s; {
        if dref s >= '0' && dref s <= '9'; {
            digits += 1;
        }
        s += 1;
    }

    exit digits as u8;
}

:/ OUTPUT
@exit_code=3
/:
