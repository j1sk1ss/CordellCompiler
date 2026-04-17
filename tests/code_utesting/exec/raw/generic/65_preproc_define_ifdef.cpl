: BLOCK_TEST :
: RUN_ASM :
#define FLAG 0
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
#ifdef FLAG
    putc('D');
#endif
#ifndef FLAG
    putc('X');
#endif
    exit 0;
}

:/ OUTPUT
D
/:
