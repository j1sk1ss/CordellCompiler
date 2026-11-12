: BLOCK_TEST :
: RUN_ASM[args="x"|args="1234567"|args="forty-two"] :

function strlen432(ptr i8 text) -> i32 {
    i32 length = 0;
    while text[length]; { length += 1; }
    return length;
}

start(i64 argc, ptr ptr i8 argv) {
    exit (strlen432(argv[1]) + argc as i32) as u8;
}

:/ OUTPUT
@case_index=0
@exit_code=3
---
@case_index=1
@exit_code=9
---
@case_index=2
@exit_code=11
---
/:
