: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    arr words[2, ptr i8] = { ref "AX", ref "BY" };
    putc(words[0][1]);
    putc(words[1][0]);
    exit 0;
}

:/ OUTPUT
XB
/:
