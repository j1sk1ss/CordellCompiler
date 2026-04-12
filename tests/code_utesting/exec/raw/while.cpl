: RUN_ASM[args="","",""|args=""|args="","","",""] :
start(i32 argc) {
    while argc; {
        syscall(0x2000004, 1, ref "Iteration!\n", 11);
        argc -= 1;
    }
}

:/ OUTPUT
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
/: