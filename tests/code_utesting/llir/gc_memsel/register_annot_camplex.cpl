#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    @[register(RDX)] i32 b = 1 as i32;
    exit (a + b) as i8;
}

:/ OUTPUT
BB1: start
rdx = $1;
rbx = rdx;
r8 = $1;
rbx = r8;
rax = rbx;
rax = rax + rbx;
rdi = rax;
r9 = rdi;
rdx = r9;
exit rdx;
BB2:
/: