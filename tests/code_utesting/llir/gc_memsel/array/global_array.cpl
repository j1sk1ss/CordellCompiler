glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
rdi = $1;
rcx = &(%a);
rax = $0;
rax = rax * 4;
rbx movsx rax;
rdx = rbx;
rax = rcx;
rax = rax + rdx;
rsi = rax;
*(rsi) = rdi;
send
/: