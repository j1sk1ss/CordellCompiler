#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    @[register(RDX)] i32 b = 1 as i32;
    exit (a + b) as i8;
}

:/ OUTPUT
BB1: start
rcx = $1;
rbx = rcx;
rcx = $1;
rbx = rcx;
rax = rbx;
rax = rax + rbx;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB2: send
/: