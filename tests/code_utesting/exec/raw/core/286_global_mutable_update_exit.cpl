: BLOCK_TEST :
: RUN_ASM :

glob i32 counter = 40;

function bump() -> i0 {
    counter += 2;
}

start() {
    bump();

    exit counter as u8;
}

:/ OUTPUT
@exit_code=42
/:
