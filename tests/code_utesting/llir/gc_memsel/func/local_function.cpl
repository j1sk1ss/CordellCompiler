function foo() {
    i32 a = 1 as i32;
    i32 b = 1 as i32;
    function bar() {
        i32 a = 1 as i32;
        i32 b = 1 as i32;
        return a + b;
    }
    return a + b + bar();
}

start() {
    foo();
}

:/ OUTPUT
BB3: fn foo0()
r9 = $1;
r8 = r9;
rdx = $1;
rsi = rdx;
jmp lb28;
BB4: lb28:
rax = r8;
rax = rax + rsi;
r10 = rax;
call bar1();
rcx = rax;
rbx movsxd r10;
rax = rbx;
rax = rax + rcx;
rdi = rax;
rax = rdi;
return rax;
BB5:
BB6: fn bar1()
rsi = $1;
rbx = rsi;
rcx = $1;
rdi = rcx;
rax = rbx;
rax = rax + rdi;
rdx = rax;
rax = rdx;
return rax;
BB7:
BB8: start
call foo0();
send
/: