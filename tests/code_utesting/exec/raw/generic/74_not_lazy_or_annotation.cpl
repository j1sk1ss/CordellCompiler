: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

glob i64 counter = 0;

function mark() -> i64 {
    counter += 1;
    return 1;
}

start() {
    if @[not_lazy] (1 || mark()); {
    }
    put_digit(counter);
    exit 0;
}

:/ OUTPUT
1
/:
