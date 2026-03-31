#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    @[register(RDX)] i32 b = 1 as i32;
    exit (a + b) as i8;
}

:/ OUTPUT
BB1: start
r15 = $1;
rsi = r15;
r15 = rsi;
rdx = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
rdx = r15;
r15 = rdx;
rax = r15;
rax = rax + rdx;
r15 = rax;
rdi = r15;
r15 = rdi;
r8 = r15;
r15 = r8;
rdx = r15;
exit rdx;
BB2:
/:
