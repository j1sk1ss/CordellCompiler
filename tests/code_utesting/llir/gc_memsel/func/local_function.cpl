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
r10 = $1;
r9 = r10;
rbx = $1;
rdi = rbx;
jmp lb28;
BB4: lb28:
rax = r9;
rax = rax + rdi;
r11 = rax;
call bar1();
rdx = rax;
rsi movsxd r11;
rax = rsi;
rax = rax + rdx;
r8 = rax;
rax = r8;
return rax;
BB5:
BB6: fn bar1()
rdi = $1;
rsi = rdi;
rdx = $1;
r8 = rdx;
rax = rsi;
rax = rax + r8;
rbx = rax;
rax = rbx;
return rax;
BB7:
BB8: start
call foo0();
send
/: