: RUN_ASM :
start() {
    @[no_fall]
    switch 2; {
        case 1; {
            syscall(0x2000004, 1, ref "case 1!\n", 8);
        }
        case 2; {
            syscall(0x2000004, 1, ref "case 2!\n", 8);
        }
        default {
            syscall(0x2000004, 1, ref "default!\n", 9);
        }
    }
}

:/ OUTPUT
case 2!
/: