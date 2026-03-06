{
    function foo(i32 a, i32 b) {
    }
    start() {
        i32 a = 1;
        foo(1, 1);
        i32 b = a + 1;
        exit b;
    }
}

: OUTPUT
BB2: fn foo0(i32 a, i32 b)
rdi = rdi;
rax = rdi;
rsi = rsi;
rax = rsi;
[unused] kill(15);
[unused] kill(9);
[unused] kill(8);
[unused] kill(14);
[unused] kill(13);
[unused] kill(12);
fend
BB3: start
rbx = $1;
ecx = rbx;
eax = ecx;
rsp = eax;
use $1;
use $1;
r8 = $1;
rdi <<= r8;
ebx = $1;
rsi <<= ebx;
push(rax);
call foo0(i32 a, i32 b);
rax = pop();
r11 movsxd rsp;
rcx = r11;
rax = rcx;
rax = rax + $1;
rax = rax;
r10 = rax;
r15 = r10;
r9 = r15;
r12 = r9;
r13 = r12;
r14 = r13;
rdx = r14;
[unused] kill(17);
[unused] kill(4);
[unused] kill(23);
[unused] kill(21);
[unused] kill(26);
[unused] kill(28);
[unused] kill(27);
[unused] kill(11);
[unused] kill(16);
[unused] kill(5);
[unused] kill(6);
[unused] kill(19);
[unused] kill(7);
[unused] kill(22);
[unused] kill(18);
[unused] kill(10);
[unused] kill(0);
[unused] kill(25);
[unused] kill(24);
[unused] kill(20);
exit rdx;
BB4:
: