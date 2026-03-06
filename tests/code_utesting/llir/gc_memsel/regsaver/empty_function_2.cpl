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
rbp = $1;
r12 = rbp;
rsp = r12;
r11 = rsp;
call foo0();
r10 movsxd r11;
rbx = r10;
rax = rbx;
rax = rax + $1;
rax = rax;
r9 = rax;
rsi = r9;
rcx = rsi;
r13 = rcx;
r8 = r13;
rdi = r8;
rdx = rdi;
[unused] kill(8);
[unused] kill(2);
[unused] kill(5);
[unused] kill(15);
[unused] kill(9);
[unused] kill(4);
[unused] kill(3);
[unused] kill(7);
[unused] kill(13);
[unused] kill(11);
[unused] kill(10);
[unused] kill(16);
[unused] kill(14);
[unused] kill(6);
[unused] kill(12);
exit rdx;
BB4:
: