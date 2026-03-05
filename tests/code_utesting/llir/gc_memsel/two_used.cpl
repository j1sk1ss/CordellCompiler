{
    start() {
        i32 a = 1;
        i32 b = 1;
        if a; exit b;
        else exit a;
    }
}

: OUTPUT
BB{X}: start
rbx = $1;
r9 = rbx;
rsi = r9;
rbp = rsi;
rsp = $1;
rdi = rsp;
r8 = rdi;
rcx = r8;
cmp rbp, 0;
je lb13;
[unused] kill(2);
[unused] kill(9);
[unused] kill(7);
[unused] kill(3);
[unused] kill(6);
[unused] kill(8);
jne lb12;
BB{X}: lb12:
rax = rcx;
rdx = rax;
[unused] kill(11);
[unused] kill(10);
[unused] kill(5);
exit rdx;
BB{X}: lb13:
rax = rbp;
rdx = rax;
[unused] kill(4);
[unused] kill(13);
[unused] kill(12);
exit rdx;
BB{X}:
: