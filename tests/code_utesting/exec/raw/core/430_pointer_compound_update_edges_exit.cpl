: BLOCK_TEST :
: RUN_ASM :

start() {
    arr values[4, i32];
    values[0] = 2;
    values[1] = 7;
    values[2] = 13;
    values[3] = 20;

    ptr i32 p = ref values;
    p += (3 * sizeof(i32));

    i32 total = dref p;
    p -= (2 * sizeof(i32));
    
    total += dref p;
    p += sizeof(i32);
    
    total += dref p;
    exit (total + 2) as u8;
}

:/ OUTPUT
@exit_code=42
/:
