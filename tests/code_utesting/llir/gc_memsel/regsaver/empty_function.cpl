{
    function foo() {
        return 1;
    }
    start() {
        i32 a = 1;
        foo();
        i32 b = a + 1;
        exit b;
    }
}

: OUTPUT
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
r9 movsxd r8;
r15 = r9;
rax = r15;
rax = rax + $1;
r15 = rax;
rdx = r15;
r15 = rdx;
rdi = r15;
r15 = rdi;
rcx = r15;
r15 = rcx;
rdx = r15;
exit rdx;
BB5:
: