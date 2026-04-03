start() {
    i32 a = 1 as i32;
    a += 1 as i32;
    exit a;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
r8 = $1;
rdi = r8;
rcx = $1;
rax = rdi;
rax = rax + rcx;
rsi = rax;
rdx = rsi;
rdx = rdx;
exit rdx;
BB2:
=== AFTER ===
BB1: start
r8 = $1;
rdi = $1;
rcx = $1;
rax = $1;
rax = rax + rcx;
rsi = rax;
rdx = rax;
exit rdx;
BB2:
/: