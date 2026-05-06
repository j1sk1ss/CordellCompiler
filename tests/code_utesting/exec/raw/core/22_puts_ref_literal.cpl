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

function puts(ptr i8 s) -> i0 {
    syscall(0x2000004, 1, s, strlen(s));
}

start() {
    puts(ref "Hi");
    exit 0;
}

:/ OUTPUT
Hi
/:
