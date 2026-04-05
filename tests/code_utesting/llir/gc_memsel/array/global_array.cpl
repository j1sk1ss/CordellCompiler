glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
r8 = $1;
rdx = &(%a);
rax = $0;
rax = rax * 4;
rsi movsx rax;
rbx = rsi;
rax = rdx;
rax = rax + rbx;
rdi = rax;
*(rdi) = r8;
send
/: