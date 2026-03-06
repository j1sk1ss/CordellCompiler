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
rdi = rdi;
rbp = rdi;
rsi movsxd rbp;
rdx = rsi;
rax = rdx;
rax = rax + $10;
rax = rax;
rcx = rax;
rbx = rcx;
rax = rbx;
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
rbp = $1;
r10 = rbp;
r15 = r10;
rsi = r15;
use rsi;
rbx = rsi;
rdi <<= rbx;
push(rsi);
push(rdx);
push(rax);
push(rcx);
call foo0(i32 a);
rcx = pop();
rax = pop();
rdx = pop();
rsi = pop();
rsp movsxd rsi;
rcx = rsp;
rax = rcx;
rax = rax + $1;
rax = rax;
r9 = rax;
r11 = r9;
r8 = r11;
r13 = r8;
r12 = r13;
r14 = r12;
rdx = r14;
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