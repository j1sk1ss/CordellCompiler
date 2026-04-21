start() {
    arr a[10, i32];
    a[0] = 1 as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rsi = $1;
rbx = &([rbp - 40]);
rax = $0;
rax = rax * 4;
rdx = rax;
rdx movsx rdx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
*(rcx) = rsi;
send
=== AFTER ===
BB1: start
rsi = $1;
rbx = &([rbp - 40]);
rax = rax ^ rax;
rax = rax * 4;
rdx = rax;
rdx movsx rdx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
*(rcx) = rsi;
send
/: