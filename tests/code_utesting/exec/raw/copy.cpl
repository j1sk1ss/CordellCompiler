: RUN_ASM :
start() {
    arr dst[13, i8];
    str src = "Hello world!\n";
    
    i32 index = 0;
    while index < 13; {
        dst[index] = src[index];
        index += 1;
    }

    syscall(0x2000004, 1, ref dst, 13);
}

:/ OUTPUT
Hello world!
/: