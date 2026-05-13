start() {
    i32 a = 1;
    i32 b = 2;
    exit (a + b) as u8;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rcx = $1;
rcx = rcx;
rdx = $2;
rdx = rdx;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB2: send
=== AFTER ===
BB1: start
rcx = $1;
rcx = rcx + $2;
rdi = rcx;
exit rdi;
BB2: send
/: