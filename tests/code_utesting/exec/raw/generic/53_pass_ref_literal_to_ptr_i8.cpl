: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function first(ptr i8 msg) -> i8 {
    return msg[0];
}

start() {
    putc(first(ref "Hello"));
    exit 0;
}

:/ OUTPUT
H
/:
