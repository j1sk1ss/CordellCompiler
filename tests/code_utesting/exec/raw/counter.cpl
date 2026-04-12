: RUN_ASM :
start() {
    str msg = "Hello world!\n";
    @[counter(10)] loop {
        syscall(0x2000004, 1, ref msg, 13);
    }
}

:/ OUTPUT
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
/: