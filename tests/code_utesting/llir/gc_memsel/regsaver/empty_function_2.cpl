{
    function foo() {
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
fend
BB3: start
r15 = $1;
r9 = r15;
r15 = r9;
r8 = r15;
call foo0();
rdi movsxd r8;
r15 = rdi;
rax = r15;
rax = rax + $1;
r15 = rax;
rsi = r15;
r15 = rsi;
rcx = r15;
r15 = rcx;
rdx = r15;
r15 = rdx;
rdx = r15;
exit rdx;
BB4:
: