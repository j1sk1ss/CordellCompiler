: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i64 x = 2;
    if x == 1; putc('A');
    else if x == 2; putc('B');
    else putc('C');
    exit 0;
}

:/ OUTPUT
B
/:
