: RUN_ASM :
function sum<T>(T a, T b) {
    return a + b;
}

start() {
    exit (sum<i8>(100, 0) + sum<u8>(1, 0)) as u8;
}

:/ OUTPUT
@exit_code=101
/: