: BLOCK_TEST :
: RUN_ASM :

glob arr data399[4, i32];

function fill399(ptr i32 p) -> i0 {
    p[0] = 3;
    p[1] = 7;
    p[2] = 11;
    p[3] = 21;
}

start() {
    fill399(ref data399);
    exit (data399[0] + data399[1] + data399[2] + data399[3]) as u8;
}

:/ OUTPUT
@exit_code=42
/:
