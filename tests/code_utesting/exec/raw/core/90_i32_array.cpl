: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

glob arr a[16, i32];

start(i32 argc, ptr ptr i8 argv) {
    a[2] = 7;
    a[7] = 3;
    putdigit(a[7]);
    putdigit(a[2]);
    exit 0;
}

:/ OUTPUT
37
/: