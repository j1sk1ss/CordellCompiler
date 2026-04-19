function foo() {
    i32 a = 1 as i32;
    exit a + 10 as i32;
}

start() {
    i32 a = 1 as i32;
    foo();
    i32 b = 1 as i32;
    exit (a + b) as i8;
}

:/ OUTPUT
BB2: fn foo0()
rdx = $1;
rdx = rdx;
rbx = $10;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rdi = rcx;
exit rdi;
BB3:
BB4: start
rdx = $1;
rbx = rdx;
push(rbx);
push(rdx);
push(rdi);
push(rcx);
call foo0();
rcx = pop();
rdi = pop();
rdx = pop();
rbx = pop();
rdx = $1;
rdx = rdx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB5:
/: