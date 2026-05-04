: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    ptr i8 p = ref "AB";
    while dref p; {
        putc(dref p);
        p += 1 as ptr i8;
    }
    exit 0;
}

:/ OUTPUT
AB
/:
