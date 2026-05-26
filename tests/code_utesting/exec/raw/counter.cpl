: ASM_ARCH=x86_64_nasm_macho :
: RUN_ASM :
start() {
    arr msg[0, i8] = "Hello world!\n";
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