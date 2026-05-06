: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
#ifndef MISSING_FLAG
    putc('N');
#endif
#ifdef MISSING_FLAG
    putc('X');
#endif
    exit 0;
}

:/ OUTPUT
N
/:
