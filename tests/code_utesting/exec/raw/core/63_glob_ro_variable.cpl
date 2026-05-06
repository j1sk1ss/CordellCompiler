: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

glob ro i8 G = 'G';

start() {
    putc(G);
    exit 0;
}

:/ OUTPUT
G
/:
