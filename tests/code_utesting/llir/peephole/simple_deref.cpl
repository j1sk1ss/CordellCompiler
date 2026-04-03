start() {
    ptr i32 a = 0xB8400;
    dref a = 0 as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rdx = $754688;
rcx = rdx;
rax = $0;
*(rcx) = rax;
send
=== AFTER ===
BB1: start
rdx = $754688;
rcx = $754688;
rax = rax ^ rax;
*(rcx) = rax;
send
/: