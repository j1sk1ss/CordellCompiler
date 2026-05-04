: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start(i32 argc, ptr ptr i8 argv) {
    putc('A');
    exit 0;
}

:/ OUTPUT
A
/: