start() {
    arr a[10, i32];
    a[0] = 1;
}

: OUTPUT
=== BEFORE ===
BB{X} start
r15 = &([rbp - 40]);
rsi = r15;
r15 = $0;
rax = r15;
rax = rax * 4;
r15 = rax;
rdi = r15;
r15 = rdi;
rbx = r15;
r15 = rsi;
rax = r15;
rax = rax + rbx;
r15 = rax;
rdx = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
*(rdx) = r15;
kill(6);
send
=== AFTER ===
BB{X} start
r15 = &([rbp - 40]);
rsi = r15;
r15 = r15 ^ r15;
rax = r15;
rax = rax << 2;
rdi = rax;
rbx = rax;
rax = rsi;
rax = rax + rbx;
rdx = rax;
rcx = $1;
*(rdx) = $1;
kill(6);
send
: