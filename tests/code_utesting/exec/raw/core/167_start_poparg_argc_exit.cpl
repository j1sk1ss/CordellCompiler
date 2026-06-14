: BLOCK_TEST :
: RUN_ASM[args="a","b"|args="x"] :

start(i64 argc, ptr ptr i8 argv) {
    exit argc as u8;
}

:/ OUTPUT
@case_index=0
@exit_code=3
---
@case_index=1
@exit_code=2
---
/:
