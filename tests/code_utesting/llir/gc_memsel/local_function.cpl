{
    function foo() {
        i32 a = 1;
        i32 b = 1;
        function bar() {
            i32 a = 1;
            i32 b = 1;
            return a + b;
        }
        return a + b + bar();
    }
}

: OUTPUT
BB2: fn foo0()
r15 = $1;
r14 = r15;
r10 = r14;
rsp = r10;
rsi = $1;
r11 = rsi;
rbx = r11;
r12 = rbx;
[unused] kill(17);
[unused] kill(4);
[unused] kill(5);
[unused] kill(19);
[unused] kill(18);
[unused] kill(20);
jmp lb12;
BB3: lb12:
r9 = rsp;
rax = r9;
rax = rax + r12;
rax = rax;
r8 = rax;
call bar1();
rax = rax;
rcx = rax;
rdi movsxd r8;
rbp = rdi;
rax = rbp;
rax = rax + rcx;
rax = rax;
r13 = rax;
rdx = r13;
rax = rdx;
[unused] kill(9);
[unused] kill(23);
[unused] kill(21);
[unused] kill(30);
[unused] kill(29);
[unused] kill(13);
[unused] kill(26);
[unused] kill(22);
[unused] kill(28);
[unused] kill(27);
[unused] kill(11);
[unused] kill(10);
[unused] kill(25);
[unused] kill(24);
[unused] kill(14);
[unused] kill(12);
return rax;
BB4:
BB5: fn bar1()
rbp = $1;
r10 = rbp;
rdi = r10;
r8 = rdi;
rdx = $1;
rcx = rdx;
rsp = rcx;
r11 = rsp;
rsi = r8;
rax = rsi;
rax = rax + r11;
rax = rax;
r9 = rax;
rbx = r9;
rax = rbx;
[unused] kill(8);
[unused] kill(31);
[unused] kill(36);
[unused] kill(7);
[unused] kill(37);
[unused] kill(39);
[unused] kill(38);
[unused] kill(35);
[unused] kill(33);
[unused] kill(32);
[unused] kill(34);
[unused] kill(16);
[unused] kill(6);
[unused] kill(15);
return rax;
BB6:
: