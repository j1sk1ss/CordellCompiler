start() {
    i32 a = 1 as i32;
    @[register(10)] i32 b = 1 as i32;
    exit (a + b) as i32;
}

:/ OUTPUT
BB1: start
r15 = $1;
rdx = r15;
r15 = rdx;
rdi = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
r12 = r15;
r15 = rdi;
rax = r15;
rax = rax + r12;
r15 = rax;
rsi = r15;
r15 = rsi;
r8 = r15;
r15 = r8;
rdx = r15;
exit rdx;
BB2:
/:
