: RUN_ASM :

function fib(i32 n) -> i32 {
    if n < 2; {
        return n;
    }

    return fib(n - 1) + fib(n - 2);
}

start() {
    exit fib(10) as u8;
}

:/ OUTPUT
@exit_code=55
/:
