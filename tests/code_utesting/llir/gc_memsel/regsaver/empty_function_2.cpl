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
rcx = $1;
rdx = rcx;
call foo0();
r8 = $1;
rax = rdx;
rax = rax + r8;
r9 = rax;
rdi = r9;
rsi = rdi;
rdx = rsi;
exit rdx;
BB4:
/: