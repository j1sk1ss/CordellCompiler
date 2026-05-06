function foo() {
    i32 a = 1 as i32;
    return a + 10 as i32;
}

start() {
    i32 a = 1 as i32;
    i32 b = a + 1 as i32;
    foo();
}

:/ OUTPUT
BB2: fn foo0()
rcx = $1;
rcx = rcx;
rdx = $10;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rax = rcx;
return rax;
BB3: fend
BB4: start
rcx = $1;
rcx = rcx;
rdx = $1;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
call foo0();
send
/: