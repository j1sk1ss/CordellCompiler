#define RDX 2
start() {
    @[register(RDX)] i32 a = 1 as i32;
    exit (a + 1) as i8;
}

: OUTPUT
BB1: start
r15 = $1;
rdi = r15;
r15 = rdi;
rdx = r15;
rsi movsxd rdx;
r15 = rsi;
rax = r15;
rax = rax + $1;
r15 = rax;
rcx = r15;
r15 = rcx;
r8 = r15;
r15 = r8;
rdx = r15;
exit rdx;
BB2:
: