: ASM_ARCH=x86_64_nasm_macho :
: RUN_ASM :
start() {
    arr fst[0, i8] = "First option!\n";
    arr sec[0, i8] = "Second option!\n";
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