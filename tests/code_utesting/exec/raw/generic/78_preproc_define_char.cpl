: BLOCK_TEST :
: RUN_ASM :
#define CH 'M'
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    putc(CH);
    exit 0;
}

:/ OUTPUT
M
/:
