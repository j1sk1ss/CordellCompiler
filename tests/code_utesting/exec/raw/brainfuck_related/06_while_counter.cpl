: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

start(i32 argc, ptr ptr i8 argv) {
    i32 i = 0;
    while i < 5; {
        putdigit(i);
        i += 1;
    }
    exit 0;
}

:/ OUTPUT
01234
/: