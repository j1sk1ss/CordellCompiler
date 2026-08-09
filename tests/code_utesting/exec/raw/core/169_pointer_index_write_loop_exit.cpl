: BLOCK_TEST :
: RUN_ASM :

start() {
    arr data[4, i32] = { 1, 2, 3, 4 };
    ptr i32 p = (ref data) as ptr i32;
    i32 i = 0;

    while i < 4; {
        p[i] = p[i] + i;
        i += 1;
    }

    exit (data[0] + data[1] + data[2] + data[3]) as u8;
}

:/ OUTPUT
@exit_code=16
/:
