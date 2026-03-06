{
    function foo() {
        i32 a = 1;
        exit a + 10;
    }

    start() {
        i32 a = 1;
        foo();
        i32 b = 1;
        exit a + b;
    }
}

: OUTPUT
BB2: fn foo0()
rbp = $1;
rcx = rbp;
rdi = rcx;
r9 = rdi;
rsi movsxd r9;
rsp = rsi;
rax = rsp;
rax = rax + $10;
rax = rax;
r8 = rax;
rbx = r8;
rdx = rbx;
[unused] kill(17);
[unused] kill(9);
[unused] kill(15);
[unused] kill(4);
[unused] kill(3);
[unused] kill(13);
[unused] kill(18);
[unused] kill(16);
[unused] kill(14);
[unused] kill(5);
[unused] kill(12);
exit rdx;
BB3:
BB4: start
r11 = $1;
r8 = r11;
rdi = r8;
rsi = rdi;
push(r9);
push(rbx);
push(rsi);
push(rsp);
push(rax);
push(rdx);
push(rbp);
push(rcx);
call foo0();
rcx = pop();
rbp = pop();
rdx = pop();
rax = pop();
rsp = pop();
rsi = pop();
rbx = pop();
r9 = pop();
r12 = $1;
rbp = r12;
rbx = rbp;
r10 = rbx;
rcx = rsi;
rax = rcx;
rax = rax + r10;
rax = rax;
rsp = rax;
r9 = rsp;
rdx = r9;
[unused] kill(19);
[unused] kill(20);
[unused] kill(23);
[unused] kill(21);
[unused] kill(7);
[unused] kill(26);
[unused] kill(22);
[unused] kill(11);
[unused] kill(27);
[unused] kill(10);
[unused] kill(25);
[unused] kill(24);
[unused] kill(6);
[unused] kill(8);
exit rdx;
BB5:
: