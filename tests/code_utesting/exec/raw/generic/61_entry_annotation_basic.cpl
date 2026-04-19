: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

@[entry("_main")]
function main() -> i0 {
    putc('E');
    exit 0;
}

:/ OUTPUT
E
/:
