: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    put_digit(3 < 4);
    put_digit(4 == 4);
    put_digit(5 != 5);
    exit 0;
}

:/ OUTPUT
110
/:
