function foo() {
    return 1;
}
start() {
    i32 a = 1 as i32;
    foo();
    i32 b = a + 1 as i32;
    exit b as i8;
}

:/ OUTPUT
BB2: fn foo0()
rax = $1;
return rax;
BB3:
BB4: start
rcx = $1;
rdx = rcx;
push(rax);
call foo0();
rax = pop();
r8 = $1;
rax = rdx;
rax = rax + r8;
r9 = rax;
rdi = r9;
rsi = rdi;
rdx = rsi;
exit rdx;
BB5:
/: