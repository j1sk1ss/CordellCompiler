: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i64 x = 5;
    if x > 10; putc('A');
    else putc('B');
    exit 0;
}

:/ OUTPUT
B
/:
