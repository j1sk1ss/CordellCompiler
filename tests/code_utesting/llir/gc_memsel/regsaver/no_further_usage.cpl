{
    function foo() {
        i32 a = 1;
        return a + 10;
    }

    start() {
        i32 a = 1;
        i32 b = a + 1;
        foo();
    }
}

: OUTPUT
BB2: fn foo0()
rax = $1;
rdx = rax;
rax = rdx;
rcx = rax;
rbx movsxd rcx;
rax = rbx;
rax = rax;
rax = rax + $10;
rax = rax;
rsi = rax;
rax = rsi;
rax = rax;
[unused] kill(17);
[unused] kill(19);
[unused] kill(4);
[unused] kill(3);
[unused] kill(13);
[unused] kill(18);
[unused] kill(10);
[unused] kill(16);
[unused] kill(5);
[unused] kill(14);
[unused] kill(15);
return rax;
BB3:
BB4: start
rax = $1;
rsi = rax;
rax = rsi;
rdi = rax;
rdx movsxd rdi;
rax = rdx;
rax = rax;
rax = rax + $1;
rax = rax;
rcx = rax;
rax = rcx;
r8 = rax;
rax = r8;
rbx = rax;
call foo0();
[unused] kill(20);
[unused] kill(9);
[unused] kill(12);
[unused] kill(23);
[unused] kill(21);
[unused] kill(7);
[unused] kill(26);
[unused] kill(22);
[unused] kill(11);
[unused] kill(25);
[unused] kill(24);
[unused] kill(6);
[unused] kill(8);
send
: