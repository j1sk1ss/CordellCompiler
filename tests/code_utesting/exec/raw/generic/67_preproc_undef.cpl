: BLOCK_TEST :
: RUN_ASM :
#define TEMP_FLAG 0
#undef TEMP_FLAG
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
#ifndef TEMP_FLAG
    putc('U');
#endif
#ifdef TEMP_FLAG
    putc('X');
#endif
    exit 0;
}

:/ OUTPUT
U
/:
