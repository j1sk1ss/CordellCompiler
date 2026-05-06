: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 c = 0;

    @[counter(4)] loop {
        c += 1;
    }

    put_digit(c);
    exit 0;
}

:/ OUTPUT
4
/:
