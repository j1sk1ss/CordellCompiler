: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    ptr i8 s = ref "ABC";
    putc(s[1]);
    exit 0;
}

:/ OUTPUT
B
/:
