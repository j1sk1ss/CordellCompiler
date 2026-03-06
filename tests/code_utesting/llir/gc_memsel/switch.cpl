{
    start() {
        @[straight]
        switch 1; {
            case 1; { exit 0; }
            case 2; { exit 1; }
            default { exit 2; }
        }
    }
}

: OUTPUT
BB1: start
jmp lb4;
BB2: lb6:
rbx = $0;
rdx = rbx;
[unused] kill(3);
[unused] kill(2);
exit rdx;
BB3: lb10:
rbx = $1;
rdx = rbx;
[unused] kill(4);
[unused] kill(5);
exit rdx;
BB4: lb14:
rbx = $2;
rdx = rbx;
[unused] kill(7);
[unused] kill(6);
exit rdx;
BB5: lb4:
rbp = $1;
rax = rbp;
cmp rax, 1;
sete al;
rdi movzx al;
cmp rdi, 0;
je lb19;
[unused] kill(9);
[unused] kill(0);
[unused] kill(8);
jne lb6;
BB6: lb19:
rcx = $1;
rax = rcx;
cmp rax, 2;
sete al;
rsi movzx al;
cmp rsi, 0;
je lb22;
[unused] kill(11);
[unused] kill(1);
[unused] kill(10);
jne lb10;
BB7: lb22:
jmp lb14;
BB8: lb5:
: