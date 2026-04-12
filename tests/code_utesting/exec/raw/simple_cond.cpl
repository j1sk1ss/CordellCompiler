: RUN_ASM :
start() {
    str fst = "First option!\n";
    str sec = "Second option!\n";
    if 1; {
        syscall(0x2000004, 1, ref fst, 14);
    }
    else {
        syscall(0x2000004, 1, ref sec, 15);
    }
}

:/ OUTPUT
First option!
/: