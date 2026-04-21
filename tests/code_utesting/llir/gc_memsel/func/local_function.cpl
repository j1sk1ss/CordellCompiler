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
rdx movsxd rdx;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rax = rcx;
return rax;
BB5:
BB6: fn bar1()
rdx = $1;
rbx = rdx;
rdx = $1;
rdx = rdx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rax = rcx;
return rax;
BB7:
BB8: start
call foo0();
send
/: