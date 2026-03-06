{
    function foo(i32 a) {
        i32 b = a;
        i32 c = b + a + 10;
        return a + c;
    }

    start() {
        i32 a = 1;
        foo(a);
        i32 b = a + 10;
        exit b;
    }
}

: OUTPUT
BB2: fn foo0(i32 a)
rdi = rdi;
r15 = rdi;
r8 = r15;
r11 = r8;
rbp = r11;
rax = rbp;
rax = rax + r15;
rax = rax;
r12 = rax;
rcx movsxd r12;
rsi = rcx;
rax = rsi;
rax = rax + $10;
rax = rax;
r14 = rax;
rbx = r14;
r13 = rbx;
r10 = r13;
eax = r10;
rdx = r15;
rax = rdx;
rax = rax + eax;
rax = rax;
rsp = rax;
r9 = rsp;
rax = r9;
[unused] kill(9);
[unused] kill(23);
[unused] kill(21);
[unused] kill(29);
[unused] kill(26);
[unused] kill(28);
[unused] kill(27);
[unused] kill(32);
[unused] kill(16);
[unused] kill(5);
[unused] kill(6);
[unused] kill(15);
[unused] kill(19);
[unused] kill(8);
[unused] kill(31);
[unused] kill(14);
[unused] kill(7);
[unused] kill(30);
[unused] kill(22);
[unused] kill(33);
[unused] kill(34);
[unused] kill(25);
[unused] kill(24);
[unused] kill(20);
return rax;
BB3:
BB4: start
r13 = $1;
rcx = r13;
rbp = rcx;
r15 = rbp;
use r15;
r9 = r15;
rdi <<= r9;
push(r8);
push(r15);
push(rbx);
push(rsi);
push(rsp);
push(rdx);
push(r11);
push(r10);
push(rax);
push(r14);
push(r12);
call foo0(i32 a);
r12 = pop();
r14 = pop();
rax = pop();
r10 = pop();
r11 = pop();
rdx = pop();
rsp = pop();
rsi = pop();
rbx = pop();
r15 = pop();
r8 = pop();
rsp movsxd r15;
rbx = rsp;
rax = rbx;
rax = rax + $10;
rax = rax;
r12 = rax;
r8 = r12;
r10 = r8;
r14 = r10;
r11 = r14;
rsi = r11;
rdx = rsi;
[unused] kill(17);
[unused] kill(36);
[unused] kill(42);
[unused] kill(39);
[unused] kill(44);
[unused] kill(38);
[unused] kill(45);
[unused] kill(13);
[unused] kill(43);
[unused] kill(37);
[unused] kill(40);
[unused] kill(18);
[unused] kill(35);
[unused] kill(11);
[unused] kill(10);
[unused] kill(41);
[unused] kill(12);
exit rdx;
BB5:
: