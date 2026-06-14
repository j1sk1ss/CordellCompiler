: BLOCK_TEST :
: RUN_ASM :

glob i32 counter = 0;

function mark() -> i32 {
    counter += 1;
    return 1;
}

start() {
    if @[not_lazy] (0 && mark()); {
        counter += 10;
    }

    exit counter as u8;
}

:/ OUTPUT
@exit_code=1
/:
