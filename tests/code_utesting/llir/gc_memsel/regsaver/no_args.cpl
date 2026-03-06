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
rax = $1;
rcx = rax;
rax = rcx;
rdi = rax;
rdx movsxd rdi;
rax = rdx;
rax = rax;
rax = rax + $10;
rax = rax;
rsi = rax;
rax = rsi;
rdx = rax;
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
rax = $1;
rdi = rax;
rax = rdi;
rcx = rax;
push(rsi);
push(rdx);
push(rax);
push(rcx);
call foo0();
rcx = pop();
rax = pop();
rdx = pop();
rsi = pop();
rax = $1;
rdx = rax;
rax = rdx;
r8 = rax;
rax = rcx;
rax = rax;
rax = rax + r8;
rax = rax;
rsi = rax;
rax = rsi;
rdx = rax;
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