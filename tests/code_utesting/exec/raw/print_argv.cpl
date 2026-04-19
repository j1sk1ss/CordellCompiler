: RUN_ASM[args="Hello world!"|args="World hello!"|args="123456789123"] :
start(i32 argc, ptr ptr i8 argv) {
    syscall(0x2000004, 1, argv[1 as i32], 12);
}

:/ OUTPUT
@case_index=0
Hello world!
---
@case_index=1
World hello!
---
@case_index=2
123456789123
---
/: