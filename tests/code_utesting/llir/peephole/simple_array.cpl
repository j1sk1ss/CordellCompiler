start() {
    arr a[10, i32];
    a[0] = 1 as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rdi = $1;
r8 = &([rbp - 40]);
rax = $0;
rax = rax * 4;
rbx movsx rax;
rdx = rbx;
rax = r8;
rax = rax + rdx;
rsi = rax;
*(rsi) = rdi;
send
=== AFTER ===
BB1: start
rdi = $1;
r8 = &([rbp - 40]);
rax = rax ^ rax;
rax = rax << 2;
rbx movsx rax;
rdx = rax;
rax = r8;
rax = rax + rdx;
rsi = rax;
*(rsi) = $1;
send
/: