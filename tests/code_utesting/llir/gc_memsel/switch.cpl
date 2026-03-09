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
r15 = $0;
rdx = r15;
exit rdx;
BB3: lb10:
r15 = $1;
rdx = r15;
exit rdx;
BB4: lb14:
r15 = $2;
rdx = r15;
exit rdx;
BB5: lb4:
r15 = $1;
rax = r15;
cmp rax, 1;
sete al;
rdx movzx al;
cmp rdx, 0;
je lb19;
jne lb6;
BB6: lb19:
r15 = $1;
rax = r15;
cmp rax, 2;
sete al;
rcx movzx al;
cmp rcx, 0;
je lb22;
jne lb10;
BB7: lb22:
jmp lb14;
BB8: lb5:
: