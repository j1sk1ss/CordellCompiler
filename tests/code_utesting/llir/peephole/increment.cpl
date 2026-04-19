start() {
    i32 a = 1 as i32;
    a += 1 as i32;
    exit a;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rdx = $1;
rdx = rdx;
rbx = $1;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB2:
=== AFTER ===
BB1: start
rdx = $1;
rbx = $1;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rdi = rcx;
exit rdi;
BB2:
/: