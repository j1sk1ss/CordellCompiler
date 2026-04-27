start() {
    i32 a = 1 as i32;
    a += 1 as i32;
    exit a;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rcx = $1;
rcx = rcx;
rdx = $1;
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
rdx = $1;
rax = rcx;
rcx = rcx + rdx;
rdi = rcx;
exit rdi;
BB2: send
/: