: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    putc('A');
    putc('B');
    putc('C');
}

:/ OUTPUT
ABC
/: