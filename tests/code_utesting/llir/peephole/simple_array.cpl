start() {
    arr a[10, i32];
    a[0] = 1 as i32;
}

: OUTPUT
=== BEFORE ===
BB1: start
r15 = $1;
rsi = r15;
r15 = &([rbp - 40]);
rdi = r15;
r15 = $0;
rax = r15;
rax = rax * 4;
r15 = rax;
rdx = r15;
r15 = rdx;
rcx = r15;
r15 = rdi;
rax = r15;
rax = rax + rcx;
r15 = rax;
rbx = r15;
r15 = rsi;
*(rbx) = r15;
kill(6);
send
=== AFTER ===
BB1: start
r15 = $1;
rsi = $1;
r15 = &([rbp - 40]);
rdi = r15;
r15 = r15 ^ r15;
rax = r15;
rax = rax << 2;
rdx = rax;
rcx = rax;
rax = rdi;
rax = rax + rcx;
rbx = rax;
r15 = $1;
*(rbx) = $1;
kill(6);
send
: