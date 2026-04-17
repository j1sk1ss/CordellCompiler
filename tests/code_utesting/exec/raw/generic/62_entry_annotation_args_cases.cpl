: BLOCK_TEST :
: RUN_ASM[args="apple"|args="banana"] :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

@[entry("_main")]
function main(i32 argc, ptr ptr i8 argv) -> i0 {
    putc(argv[1][0]);
    exit 0;
}

:/ OUTPUT
@case_index=0
a
---
@case_index=1
b
---
/:
