start() {
    arr a[10, i32];
    a[0] = 1 as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rsi = $1;
rdi = &([rbp - 40]);
rax = $0;
rax = rax * 4;
rdx movsx rax;
rcx = rdx;
rax = rdi;
rax = rax + rcx;
rbx = rax;
*(rbx) = rsi;
kill(6);
send
=== AFTER ===
BB1: start
rsi = $1;
rdi = &([rbp - 40]);
rax = rax ^ rax;
rax = rax << 2;
rdx movsx rax;
rcx = rax;
rax = rdi;
rax = rax + rcx;
rbx = rax;
*(rbx) = $1;
kill(6);
send
/: