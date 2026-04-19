: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}


start() {
    i64 out = 0;
    asm(out) {
        "mov %0, 65"
    }
    putc(out as i8);
    exit 0;
}

:/ OUTPUT
A
/:
