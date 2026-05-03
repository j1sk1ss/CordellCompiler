start() {
    i32 a = (10 * 1024) as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rcx = $10;
rax movsx rcx;
rax = rax * $1024;
rcx = rax;
rcx movsx rcx;
rcx = rcx;
send
=== AFTER ===
BB1: start
send
/: