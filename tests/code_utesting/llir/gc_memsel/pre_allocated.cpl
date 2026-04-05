start() {
    i32 a = 1 as i32;
    @[register(10)] i32 b = 1 as i32;
    exit (a + b) as i32;
}

:/ OUTPUT
BB1: start
rdx = $1;
rbx = rdx;
r8 = $1;
r13 = r8;
rax = rbx;
rax = rax + r13;
rdi = rax;
r9 = rdi;
rdx = r9;
exit rdx;
BB2:
/: