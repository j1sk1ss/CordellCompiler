: BLOCK_TEST :
: RUN_ASM :
start() {
    i8 c = 'S';
    syscall(0x2000004, 1, ref c, 1);
    exit 0;
}

:/ OUTPUT
S
/:
