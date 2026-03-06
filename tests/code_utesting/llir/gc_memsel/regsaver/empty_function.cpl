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
rbx = $1;
rax = rbx;
[unused] kill(9);
[unused] kill(8);
return rax;
BB3:
BB4: start
rsp = $1;
rdi = rsp;
r11 = rdi;
r8 = r11;
push(rbx);
push(rax);
call foo0();
rax = pop();
rbx = pop();
r12 movsxd r8;
r9 = r12;
rax = r9;
rax = rax + $1;
rax = rax;
rsi = rax;
r10 = rsi;
rbp = r10;
r13 = rbp;
rcx = r13;
rbx = rcx;
rdx = rbx;
[unused] kill(15);
[unused] kill(12);
[unused] kill(5);
[unused] kill(17);
[unused] kill(4);
[unused] kill(3);
[unused] kill(7);
[unused] kill(13);
[unused] kill(18);
[unused] kill(11);
[unused] kill(10);
[unused] kill(16);
[unused] kill(14);
[unused] kill(6);
[unused] kill(2);
exit rdx;
BB5:
: