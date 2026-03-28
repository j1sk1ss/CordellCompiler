start() {
    i32 a = (10 * 1024) as i32;
}

: OUTPUT
=== BEFORE ===
BB1: start
r15 = $10;
rax = r15;
rax = rax * $1024;
r15 = rax;
rbx = r15;
r15 = rbx;
rdx = r15;
r15 = rdx;
rcx = r15;
send
=== AFTER ===
BB1: start
rax = $10;
rax = rax << 10;
rbx = rax;
rdx = rax;
rcx = rax;
send
: