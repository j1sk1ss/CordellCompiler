function foo() {
}
start() {
    i32 a = 1 as i32;
    foo();
    i32 b = a + 1 as i32;
    exit b as i8;
}

:/ OUTPUT
BB2: fn foo0()
fend
BB3: start
rdx = $1;
rdx = rdx;
call foo0();
rbx = $1;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rcx = rcx;
rcx = rcx;
rdi = rcx;
exit rdi;
BB4:
/: