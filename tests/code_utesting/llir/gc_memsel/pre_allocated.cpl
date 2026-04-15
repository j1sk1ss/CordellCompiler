start() {
    i32 a = 1 as i32;
    @[register(10)] i32 b = 1 as i32;
    exit (a + b) as i32;
}

:/ OUTPUT
BB1: start
rdx = $1;
rbx = rdx;
rdx = $1;
r13 = rdx;
rax = rbx;
rax = rax + r13;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB2:
/: