{
    function foo(i32 a) {
        return a + 10;
    }

    start() {
        i32 a = 1;
        foo(a);
        i32 b = a + 1;
        exit b;
    }
}

: OUTPUT
BB2: fn foo0(i32 a)
rax = rdi;
rbx = rax;
rdx movsxd rbx;
rax = rdx;
rax = rax;
rax = rax + $10;
rax = rax;
rcx = rax;
rax = rcx;
rax = rax;
[unused] kill(17);
[unused] kill(9);
[unused] kill(12);
[unused] kill(4);
[unused] kill(3);
[unused] kill(13);
[unused] kill(18);
[unused] kill(16);
[unused] kill(14);
[unused] kill(15);
return rax;
BB3:
BB4: start
rax = $1;
r9 = rax;
rax = r9;
rcx = rax;
use rcx;
rax = rcx;
rdi <<= rax;
push(rdx);
push(rax);
push(rcx);
call foo0(i32 a);
rcx = pop();
rax = pop();
rdx = pop();
rdx movsxd rcx;
rax = rdx;
rax = rax;
rax = rax + $1;
rax = rax;
r8 = rax;
rax = r8;
rsi = rax;
rax = rsi;
r10 = rax;
rax = r10;
rdx = rax;
[unused] kill(8);
[unused] kill(19);
[unused] kill(23);
[unused] kill(7);
[unused] kill(21);
[unused] kill(29);
[unused] kill(26);
[unused] kill(22);
[unused] kill(28);
[unused] kill(27);
[unused] kill(11);
[unused] kill(10);
[unused] kill(25);
[unused] kill(24);
[unused] kill(5);
[unused] kill(6);
[unused] kill(20);
exit rdx;
BB5:
: