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
rcx = $1;
rcx = rcx;
rdx = $10;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rdi = rcx;
exit rdi;
BB3: fend
BB4: start
rcx = $1;
rbx = rcx;
push(rdx);
push(rdi);
push(rcx);
call foo0();
rcx = pop();
rdi = pop();
rdx = pop();
rcx = $1;
rdx = rcx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB5: send
/: