start() {
    i32 a = 1;
    @[register(10)] i32 b = 1;
    exit a + b;
}

: OUTPUT
BB1: start
r15 = $1;
rdi = r15;
r15 = rdi;
rcx = r15;
r15 = $1;
rdx = r15;
r15 = rdx;
r12 = r15;
r15 = rcx;
rax = r15;
rax = rax + r12;
r15 = rax;
rsi = r15;
r15 = rsi;
rdx = r15;
exit rdx;
BB2:
: