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
rbx = $1;
rdi = rbx;
rsi = $10;
rax = rdi;
rax = rax + rsi;
rdx = rax;
rax = rdx;
return rax;
BB3:
BB4: start
r8 = $1;
rdx = r8;
rsi = $1;
rax = rdx;
rax = rax + rsi;
rdi = rax;
rbx = rdi;
call foo0();
send
/: