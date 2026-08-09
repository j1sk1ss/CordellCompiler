: BLOCK_TEST :
: RUN_ASM :

function id<T>(T value) -> T {
    return value;
}

function add_cast<T, U>(T a, U b) -> U {
    return a as U + b;
}

start() {
    i8 x = id<i8>(7);
    u8 y = add_cast<i8, u8>(x, 11);
    exit y;
}

:/ OUTPUT
@exit_code=18
/:
