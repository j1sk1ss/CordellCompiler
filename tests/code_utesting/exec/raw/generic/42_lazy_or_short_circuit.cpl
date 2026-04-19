: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function mark(i64 x) -> i64 {
    putc('X');
    return x;
}

start() {
    if 1 || mark(1); putc('A');
    else putc('B');
    exit 0;
}

:/ OUTPUT
A
/:
