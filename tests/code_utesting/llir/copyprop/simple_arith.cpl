start() {
    i32 a = 10;
    i32 b = 10;
    i32 c = a + b;
    i32 d = c + a;
    i32 e = d + c;
    exit e;
}

:/ OUTPUT
BB1: start
rbx = $10;
rdx = $10;
rax = rbx;
rax = rax + rdx;
rdx = rax;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rdi = rcx;
exit rdi;
BB2: send
/: