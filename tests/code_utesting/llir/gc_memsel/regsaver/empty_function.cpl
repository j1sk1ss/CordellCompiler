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
rax = $1;
rax = rax;
[unused] kill(9);
[unused] kill(8);
return rax;
BB3:
BB4: start
rax = $1;
rsi = rax;
rax = rsi;
r8 = rax;
push(rax);
call foo0();
rax = pop();
r9 movsxd r8;
rax = r9;
rax = rax;
rax = rax + $1;
rax = rax;
rdx = rax;
rax = rdx;
rdi = rax;
rax = rdi;
rcx = rax;
rax = rcx;
rdx = rax;
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