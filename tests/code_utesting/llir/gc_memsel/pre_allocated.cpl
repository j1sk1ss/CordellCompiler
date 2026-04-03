start() {
    i32 a = 1 as i32;
    @[register(10)] i32 b = 1 as i32;
    exit (a + b) as i32;
}

:/ OUTPUT
BB1: start
rcx = $1;
rdx = rcx;
rdi = $1;
r12 = rdi;
rax = rdx;
rax = rax + r12;
rsi = rax;
r8 = rsi;
rdx = r8;
exit rdx;
BB2:
/: