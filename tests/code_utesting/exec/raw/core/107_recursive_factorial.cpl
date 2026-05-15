: RUN_ASM :

function fact(i32 n) -> i32 {
    if n <= 1; {
        return 1;
    }

    return n * fact(n - 1);
}

start() {
    exit fact(5) as u8;
}

:/ OUTPUT
@exit_code=120
/:
