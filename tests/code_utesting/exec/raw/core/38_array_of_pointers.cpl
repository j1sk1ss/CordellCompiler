: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    arr a1[2, i64] = { 3, 4 };
    arr a2[1, ptr i64] = { ref a1 };
    put_digit(a2[0][1]);
    exit 0;
}

:/ OUTPUT
4
/:
