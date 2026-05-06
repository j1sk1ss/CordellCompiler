: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 a = 4;
    i64 out = 0;
    asm(a, out) {
        "push rax",
        "mov rax, %0",
        "add rax, 3",
        "mov %1, rax",
        "pop rax"
    }
    put_digit(out);
    exit 0;
}

:/ OUTPUT
7
/:
