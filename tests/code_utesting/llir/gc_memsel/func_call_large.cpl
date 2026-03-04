{
    function foo(...);
    start() {
        i32 a = 1;
        i32 b = 2;
        i32 c = 3;
        foo(a, 1, 2, b, 3, 4, c, 5, 6);
    }
}

: OUTPUT
BB1: start
ebx = $1;
r13 = ebx;
edx = r13;
ecx = edx;
r8d = $2;
esp = r8d;
r10 = esp;
r12 = r10;
r14 = $3;
r15 = r14;
rax = r15;
eax = rax;
use ecx;
use $1;
use $2;
use r12;
use $3;
use $4;
use eax;
use $5;
use $6;
edi = ecx;
rdi <<= edi;
r11 = $1;
rsi <<= r11;
ebp = $2;
rdx <<= ebp;
rbx = r12;
rcx <<= rbx;
rbp = $3;
r8 <<= rbp;
esi = $4;
r9 <<= esi;
push(eax);
push($5);
push($6);
call foo0(...);
[unused] kill(17);
[unused] kill(9);
[unused] kill(4);
[unused] kill(23);
[unused] kill(21);
[unused] kill(13);
[unused] kill(26);
[unused] kill(11);
[unused] kill(16);
[unused] kill(5);
[unused] kill(6);
[unused] kill(15);
[unused] kill(19);
[unused] kill(8);
[unused] kill(12);
[unused] kill(14);
[unused] kill(3);
[unused] kill(7);
[unused] kill(22);
[unused] kill(18);
[unused] kill(10);
[unused] kill(25);
[unused] kill(0);
[unused] kill(24);
[unused] kill(20);
send
: