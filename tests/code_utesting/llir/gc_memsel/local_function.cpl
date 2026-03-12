function foo() {
    i32 a = 1;
    i32 b = 1;
    function bar() {
        i32 a = 1;
        i32 b = 1;
        return a + b;
    }
    return a + b + bar();
}

start() {
    foo();
}

: OUTPUT
BB{X}: fn foo0()
r15 = $1;
r10 = r15;
r15 = r10;
rbx = r15;
r15 = $1;
rdi = r15;
r15 = rdi;
r8 = r15;
jmp lb12;
BB{X}: lb12:
r15 = rbx;
rax = r15;
rax = rax + r8;
r15 = rax;
rsi = r15;
call bar1();
r15 = rax;
rcx = r15;
rdx movsxd rsi;
r15 = rdx;
rax = r15;
rax = rax + rcx;
r15 = rax;
r9 = r15;
r15 = r9;
rax = r15;
return rax;
BB{X}:
BB{X}: fn bar1()
r15 = $1;
rsi = r15;
r15 = rsi;
rdx = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
rdi = r15;
r15 = rdx;
rax = r15;
rax = rax + rdi;
r15 = rax;
rbx = r15;
r15 = rbx;
rax = r15;
return rax;
BB{X}:
BB{X}: start
call foo0();
send
: