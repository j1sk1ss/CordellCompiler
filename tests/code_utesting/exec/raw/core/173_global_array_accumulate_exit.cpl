: BLOCK_TEST :
: RUN_ASM :

glob arr scores[5, i32] = { 2, 4, 6, 8, 10 };

start() {
    i32 i = 0;
    i32 sum = 0;

    while i < 5; {
        scores[i] += i;
        sum += scores[i];
        i += 1;
    }

    exit sum as u8;
}

:/ OUTPUT
@exit_code=40
/:
