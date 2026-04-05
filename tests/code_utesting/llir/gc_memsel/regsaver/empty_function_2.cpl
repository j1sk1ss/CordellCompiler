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
rbx = rdx;
call foo0();
r9 = $1;
rax = rbx;
rax = rax + r9;
r10 = rax;
r8 = r10;
rdi = r8;
rdx = rdi;
exit rdx;
BB4:
/: