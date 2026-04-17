: BLOCK_TEST :
: RUN_ASM[args="A"|args="B"] :
start(i64 argc, ptr ptr i8 argv) {
    if argv[1][0] == 'A'; {
        exit 3;
    }
    else {
        exit 7;
    }
}

:/ OUTPUT
@case_index=0
@exit_code=3
---
@case_index=1
@exit_code=7
---
/:
