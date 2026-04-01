: RUN_ASM :
start() {
    str msg = "Hello world!\n";
    syscall(0x2000004, 1, ref msg, 13);
}

:/ OUTPUT
Hello world!
/: