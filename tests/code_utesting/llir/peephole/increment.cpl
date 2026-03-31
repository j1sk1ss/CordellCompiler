start() {
    i32 a = 1 as i32;
    a += 1 as i32;
    exit a;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
r15 = $1;
rdi = r15;
r15 = rdi;
rsi = r15;
r15 = $1;
r8 = r15;
r15 = rsi;
rax = r15;
rax = rax + r8;
r15 = rax;
rdx = r15;
r15 = rdx;
rcx = r15;
r15 = rcx;
rdx = r15;
exit rdx;
BB2:
=== AFTER ===
BB1: start
rdi = $1;
rsi = $1;
r8 = $1;
rax = $1;
rax = rax + r8;
rcx = rax;
rdx = rax;
exit rdx;
BB2:
/:
