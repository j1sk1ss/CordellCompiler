#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    exit (a + 1) as i8;
}

:/ OUTPUT
BB1: start
r8 = $1;
rdx = r8;
rdi = $1;
rax = rdx;
rax = rax + rdi;
rsi = rax;
rcx = rsi;
rdx = rcx;
exit rdx;
BB2:
/: