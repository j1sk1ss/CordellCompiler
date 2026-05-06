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
BB3: fend
BB4: start
rcx = $1;
rcx = rcx;
call foo0();
rdx = $1;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rcx = rcx;
rdi = rcx;
exit rdi;
BB5: send
/: