{
    start() {
        i32 a = 1;
        @[register(10)] i32 b = 1;
        exit a + b;
    }
}

: OUTPUT
BB1: start
r11 = $1;
r9 = r11;
rsi = r9;
rcx = rsi;
rdi = $1;
rsp = rdi;
rbx = rsp;
r10 = rbx;
rbp = rcx;
rax = rbp;
rax = rax + r10;
rax = rax;
r8 = rax;
r12 = r8;
rdx = r12;
[unused] kill(2);
[unused] kill(9);
[unused] kill(14);
[unused] kill(15);
[unused] kill(8);
[unused] kill(4);
[unused] kill(3);
[unused] kill(7);
[unused] kill(13);
[unused] kill(11);
[unused] kill(10);
[unused] kill(5);
[unused] kill(6);
[unused] kill(12);
exit rdx;
BB2:
: