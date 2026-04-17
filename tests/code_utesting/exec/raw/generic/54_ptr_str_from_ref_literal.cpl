: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    ptr str a = ref "AB";
    putc(a[0]);
    putc(a[1]);
    exit 0;
}

:/ OUTPUT
AB
/:
