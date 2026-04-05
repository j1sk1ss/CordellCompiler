start() {
    i32 a = (10 * 1024) as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rbx = $10;
rax = rbx;
rax = rax * $1024;
rdx = rax;
rdi movsx rdx;
rsi = rdi;
send
=== AFTER ===
BB1: start
rbx = $10;
rax = $10;
rax = rax << 10;
rdx = rax;
rdi movsx rax;
rsi = rax;
send
/: