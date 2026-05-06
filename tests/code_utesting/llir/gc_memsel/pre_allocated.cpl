start() {
    i32 a = 1 as i32;
    @[register(10)] i32 b = 1 as i32;
    exit (a + b) as i32;
}

:/ OUTPUT
BB1: start
rcx = $1;
rdx = rcx;
rcx = $1;
r13 = rcx;
rax = rdx;
rax = rax + r13;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB2: send
/: