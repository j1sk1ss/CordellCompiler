: BLOCK_TEST :
: RUN_ASM :
function strlen(ptr i8 s) -> i64 {
    i64 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }
    return l;
}

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    put_digit(strlen(ref "ABCD"));
    exit 0;
}

:/ OUTPUT
4
/:
