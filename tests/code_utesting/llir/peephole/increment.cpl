start() {
    i32 a = 1;
    a += 1;
    exit a;
}

: OUTPUT
=== BEFORE ===
BB{X} start
r15 = $1;
rdi = r15;
r15 = rdi;
rsi = r15;
r15 = $1;
rdx = r15;
r15 = rsi;
rax = r15;
rax = rax + rdx;
r15 = rax;
r8 = r15;
r15 = r8;
rcx = r15;
r15 = rcx;
rdx = r15;
exit rdx;
BB{X}
=== AFTER ===
BB{X} start
rsi = $1;
rdx = $1;
rax = $1;
rax = rax + rdx;
rdx = rax;
exit rdx;
BB{X}
: