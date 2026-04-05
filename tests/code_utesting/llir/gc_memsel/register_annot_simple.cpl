#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    exit (a + 1) as i8;
}

:/ OUTPUT
BB1: start
r9 = $1;
rbx = r9;
r8 = $1;
rax = rbx;
rax = rax + r8;
rdi = rax;
rdx = rdi;
rdx = rdx;
exit rdx;
BB2:
/: