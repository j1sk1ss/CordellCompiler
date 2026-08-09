: BLOCK_TEST :
: RUN_ASM :

function answer(i32 n) -> i32 {
    if n == 0; {
        return 42;
    }

    return answer(n - 1);
}

start() {
    exit answer(0 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:
