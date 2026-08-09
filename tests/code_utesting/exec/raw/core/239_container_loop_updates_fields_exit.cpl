: BLOCK_TEST :
: RUN_ASM :

container counters {
    i32 even;
    i32 odd;
}

start() {
    counters c;
    c.even = 0;
    c.odd = 0;

    i32 i = 0;
    while i < 6; {
        if (i & 1) == 0; c.even += i;
        else            c.odd += i;
        i += 1;
    }

    exit (c.even + c.odd + 27) as u8;
}

:/ OUTPUT
@exit_code=42
/:
