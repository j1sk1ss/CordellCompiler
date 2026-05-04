: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

glob arr tape[4, i8];

start(i32 argc, ptr ptr i8 argv) {
    if not tape[0]; putdigit(1);
    else putdigit(0);

    tape[0] = 1 as i8;

    if not tape[0]; putdigit(1);
    else putdigit(0);

    exit 0;
}

:/ OUTPUT
10
/: