: RUN_ASM[args="Hello world!"|args="World hello!"|args="123456789123"] :
start(i32 argc, ptr ptr i8 argv) {
    syscall(0x2000004, 1, argv[1 as i32], 12);
}

:/ OUTPUT
Hello world!
World hello!
123456789123
/: