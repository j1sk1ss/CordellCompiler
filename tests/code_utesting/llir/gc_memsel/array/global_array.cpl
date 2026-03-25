glob arr a[10, i32];
start() {
    a[0] = 1;
}

: OUTPUT
%0 = arr_alloc($10);
BB{X} start
r15 = &(%a);
rsi = r15;
r15 = $0;
rax = r15;
rax = rax * 4;
r15 = rax;
rdi = r15;
r15 = rdi;
rbx = r15;
r15 = rsi;
rax = r15;
rax = rax + rbx;
r15 = rax;
rdx = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
*(rdx) = r15;
send
: