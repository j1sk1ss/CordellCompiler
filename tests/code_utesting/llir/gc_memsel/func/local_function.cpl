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
rdx = $1;
rbx = rdx;
rdx = $1;
rdx = rdx;
jmp lb17;
BB4: lb17:
rax = rbx;
rax = rax + rdx;
rdx = rax;
call bar1();
rbx = rax;
rcx movsxd rdx;
rax = rcx;
rax = rax + rbx;
rcx = rax;
rax = rcx;
return rax;
BB5: fend
BB6: fn bar1()
rcx = $1;
rdx = rcx;
rcx = $1;
rbx = rcx;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rax = rcx;
return rax;
BB7: fend
BB8: start
call foo0();
send
/: