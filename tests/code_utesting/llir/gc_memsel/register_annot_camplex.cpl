#define RDX 2
start() {
    @[register(RDX)] i32 a = 1;
    @[register(RDX)] i32 b = 1;
    exit a + b;
}

: OUTPUT
BB1: start
r15 = $1;
rdi = r15;
r15 = rdi;
rdx = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
rdx = r15;
r15 = rdx;
rax = r15;
rax = rax + rdx;
r15 = rax;
rsi = r15;
r15 = rsi;
rdx = r15;
exit rdx;
BB2:
: