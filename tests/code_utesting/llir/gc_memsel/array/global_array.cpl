glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
r15 = $1;
rsi = r15;
r15 = &(%a);
rdi = r15;
r15 = $0;
rax = r15;
rax = rax * 4;
r15 = rax;
rdx = r15;
r15 = rdx;
rcx = r15;
r15 = rdi;
rax = r15;
rax = rax + rcx;
r15 = rax;
rbx = r15;
r15 = rsi;
*(rbx) = r15;
send
/:
