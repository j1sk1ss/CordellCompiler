start() {
    ptr i32 a = 0xB8400;
    dref a = 0;
}

: OUTPUT
BB{X} start
r15 = $754688;
rdx = r15;
r15 = rdx;
rcx = r15;
r15 = $0;
rax = r15;
r15 = rax;
*(rcx) = r15;
send
=== AFTER ===
BB{X} start
rdx = $754688;
rcx = $754688;
r15 = r15 ^ r15;
rax = r15;
*(rcx) = rax;
send
: