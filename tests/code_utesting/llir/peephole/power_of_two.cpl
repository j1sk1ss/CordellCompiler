start() {
    i32 a = (10 * 1024) as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
r15 = $10;
rdx = r15;
r15 = rdx;
rax = r15;
rax = rax * $1024;
r15 = rax;
rcx = r15;
rsi movsx rcx;
r15 = rsi;
rbx = r15;
send
=== AFTER ===
BB1: start
rdx = $10;
rax = $10;
rax = rax << 10;
rcx = rax;
rsi movsx rax;
r15 = rax;
rbx = rax;
send
/: