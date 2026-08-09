: BLOCK_TEST :
: RUN_ASM :

container buffer {
    arr values[4, i32];
}

start() {
    buffer b;
    b.values[0] = 6;
    b.values[1] = 7;
    b.values[2] = 8;
    b.values[3] = 9;

    ptr i32 p = b.values;
    i32 i = 0;
    i32 sum = 0;

    while i < 4; {
        sum += p[i];
        i += 1;
    }

    exit sum as u8;
}

:/ OUTPUT
@exit_code=30
/:
