: BLOCK_TEST :
: RUN_ASM :

glob i32 calls392;

function mark392(i32 value) -> i32 {
    calls392 += 1;
    return value;
}

start() {
    calls392 = 0;
    i32 result = (0 && mark392(1)) || (1 && mark392(41));
    exit (calls392 + result) as u8;
}

:/ OUTPUT
@exit_code=42
/:
