glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
rsi = $1;
rbx = &(%a);
rax = $0;
rax = rax * 4;
rdx = rax;
rdx movsx rdx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
*(rcx) = rsi;
send
/: