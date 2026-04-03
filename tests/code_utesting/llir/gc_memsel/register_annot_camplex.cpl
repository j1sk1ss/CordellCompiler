#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    @[register(RDX)] i32 b = 1 as i32;
    exit (a + b) as i8;
}

:/ OUTPUT
BB1: start
rcx = $1;
rdx = rcx;
rdi = $1;
rdx = rdi;
rax = rdx;
rax = rax + rdx;
rsi = rax;
r8 = rsi;
rdx = r8;
exit rdx;
BB2:
/: