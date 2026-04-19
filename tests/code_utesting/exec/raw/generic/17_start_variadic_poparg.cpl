: RUN_ASM[args="abc"] :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function strlen(ptr i8 s) -> i64 {
    i64 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }
    return l;
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start(...) {
    @[poparg] i64 argc;
    @[poparg] ptr ptr i8 argv;

    put_digit(argc);
    put_digit(strlen(argv[1]));
    exit 0;
}

:/ OUTPUT
@case_index=0
23
---
/:
