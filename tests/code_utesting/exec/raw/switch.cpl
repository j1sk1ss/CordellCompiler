: RUN_ASM[args=""|args="",""|args="","",""] :
start(i32 argc, ptr ptr i8 argv) {
    @[no_fall]
    switch argc; {
        case 2; {
            syscall(0x2000004, 1, ref "case 1!\n", 8);
        }
        case 3; {
            syscall(0x2000004, 1, ref "case 2!\n", 8);
        }
        default {
            syscall(0x2000004, 1, ref "default!\n", 9);
        }
    }
}

:/ OUTPUT
@case_index=0
case 1!
---
@case_index=1
case 2!
---
@case_index=2
default!
---
/: