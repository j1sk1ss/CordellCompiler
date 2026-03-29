#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    exit (a + 1) as i8;
}

:/ OUTPUT
BB1: start
r15 = $1;
rdi = r15;
r15 = rdi;
rdx = r15;
r15 = $1;
rsi = r15;
r15 = rdx;
rax = r15;
rax = rax + rsi;
r15 = rax;
rcx = r15;
r15 = rcx;
r8 = r15;
r15 = r8;
rdx = r15;
exit rdx;
BB2:
/: