start() {
    i32 a = (10 * 1024) as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rdx = $10;
rax movsx rdx;
rax = rax * $1024;
rcx = rax;
rcx movsx rcx;
rcx = rcx;
send
=== AFTER ===
BB1: start
rdx = $10;
rax movsx rdx;
rax = rax * $1024;
send
/: