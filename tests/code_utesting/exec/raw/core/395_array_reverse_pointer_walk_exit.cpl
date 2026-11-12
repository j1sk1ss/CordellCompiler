: BLOCK_TEST :
: RUN_ASM :

start() {
    arr values[5, i32];
    i32 i = 0;
    while i < 5; {
        values[i] = i + 1;
        i += 1;
    }

    ptr i32 p = ref values;
    p += (4 * sizeof(i32));

    i32 weighted = 0;
    i = 1 as i32;
    while i <= 5; {
        weighted += (dref p) * i;
        p -= sizeof(i32);
        i += 1;
    }

    exit (weighted - 13) as u8;
}

:/ OUTPUT
@exit_code=22
/:
