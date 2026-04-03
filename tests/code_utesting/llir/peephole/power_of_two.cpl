start() {
    i32 a = (10 * 1024) as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rdx = $10;
rax = rdx;
rax = rax * $1024;
rcx = rax;
rsi movsx rcx;
rbx = rsi;
send
=== AFTER ===
BB1: start
rdx = $10;
rax = $10;
rax = rax << 10;
rcx = rax;
rsi movsx rax;
rbx = rax;
send
/: