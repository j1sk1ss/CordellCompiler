: RUN_ASM :
function sum<U, T>(U a, T b) -> T {
    return a as T + b;
}

start() {
    exit sum<i8, u8>(1, 1);
}

:/ OUTPUT
@exit_code=2
/: