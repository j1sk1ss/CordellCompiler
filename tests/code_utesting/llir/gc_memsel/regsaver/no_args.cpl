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
rdi = $1;
rbx = rdi;
r8 = $10;
rax = rbx;
rax = rax + r8;
rdx = rax;
rdx = rdx;
exit rdx;
BB3:
BB4: start
r9 = $1;
rbx = r9;
push(rbx);
push(rdx);
push(rdi);
push(r8);
call foo0();
r8 = pop();
rdi = pop();
rdx = pop();
rbx = pop();
rdi = $1;
rdx = rdi;
rax = rbx;
rax = rax + rdx;
r10 = rax;
r8 = r10;
rdx = r8;
exit rdx;
BB5:
/: