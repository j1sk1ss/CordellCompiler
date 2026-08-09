: BLOCK_TEST :
: RUN_ASM[args="A"|args="Z"|args="?"] :

start(i64 argc, ptr ptr i8 argv) {
    i8 c = argv[1][0];

    @[straight]
    @[no_fall]
    switch c; {
        case 'A'; { exit 1; }
        case 'Z'; { exit 26; }
        default  { exit 9; }
    }
}

:/ OUTPUT
@case_index=0
@exit_code=1
---
@case_index=1
@exit_code=26
---
@case_index=2
@exit_code=9
---
/:
