: RUN_ASM[args="","",""|args="",""|args="","","",""] :
start(i32 a) {
    loop {
        syscall(0x2000004, 1, ref "Iteration!\n", 11);
        a -= 1;
        if a < 0; break;
    }
}

:/ OUTPUT
@case_index=0
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
---
@case_index=1
Iteration!
Iteration!
Iteration!
Iteration!
---
@case_index=2
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
Iteration!
---
/: