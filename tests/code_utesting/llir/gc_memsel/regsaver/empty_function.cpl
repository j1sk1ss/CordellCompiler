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
r15 = $1;
rax = r15;
return rax;
BB3:
BB4: start
r15 = $1;
rsi = r15;
r15 = rsi;
r8 = r15;
push(rax);
push(r15);
call foo0();
r15 = pop();
rax = pop();
r15 = $1;
rdx = r15;
r15 = r8;
rax = r15;
rax = rax + rdx;
r15 = rax;
r9 = r15;
r15 = r9;
rcx = r15;
r15 = rcx;
rdi = r15;
r15 = rdi;
rdx = r15;
exit rdx;
BB5:
/:
