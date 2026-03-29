start() {
    @[straight]
    switch 1; {
        case 1; { exit 0; }
        case 2; { exit 1; }
        default { exit 2; }
    }
}

:/ OUTPUT
BB1: start
jmp lb10;
BB2: lb12:
r15 = $0;
rdx = r15;
exit rdx;
BB3: lb20:
r15 = $1;
rdx = r15;
exit rdx;
BB4: lb28:
r15 = $2;
rdx = r15;
exit rdx;
BB5: lb10:
r15 = $1;
rdi = r15;
r15 = rdi;
rax = r15;
cmp rax, 1;
sete al;
rsi movzx al;
cmp rsi, 0;
je lb38;
jne lb12;
BB6: lb38:
r15 = $1;
rcx = r15;
r15 = rcx;
rax = r15;
cmp rax, 2;
sete al;
rdx movzx al;
cmp rdx, 0;
je lb42;
jne lb20;
BB7: lb42:
jmp lb28;
BB8: lb11:
/: