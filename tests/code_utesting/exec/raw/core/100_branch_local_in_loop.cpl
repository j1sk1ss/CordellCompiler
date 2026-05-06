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
    i32 i = 0;

    while i < 3; {
        @[no_fall]
        @[straight]
        switch i; {
            case 1; {
                if i > 0; {
                    i32 m = 7;
                    a[i] = m;
                    a[m] = i;
                }
            }
        }
        i += 1;
    }

    putdigit(a[1]);
    putdigit(a[7]);
    exit 0;
}

:/ OUTPUT
71
/: