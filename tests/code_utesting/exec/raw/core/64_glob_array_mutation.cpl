: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

glob arr data[3, i8] = { 'A', 'B', 'C' };

start() {
    data[1] = 'Z';
    putc(data[0]);
    putc(data[1]);
    putc(data[2]);
    exit 0;
}

:/ OUTPUT
AZC
/:
