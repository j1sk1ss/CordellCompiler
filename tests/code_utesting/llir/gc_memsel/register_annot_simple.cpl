#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    exit (a + 1) as i8;
}

:/ OUTPUT
BB1: start
rcx = $1;
rbx = rcx;
rdx = $1;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB2: send
/: