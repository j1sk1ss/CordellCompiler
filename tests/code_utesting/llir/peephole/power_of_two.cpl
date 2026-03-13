start() {
    i32 a = 10 * 1024;
}

: OUTPUT
BB{X} start
r15 = $10;
rax = r15;
rax = rax * $1024;
r15 = rax;
rdx = r15;
r15 = rdx;
rbx = r15;
r15 = rbx;
rcx = r15;
send
=== AFTER ===
BB{X} start
rax = $10;
rax = rax << 10;
rcx = rax;
send
: