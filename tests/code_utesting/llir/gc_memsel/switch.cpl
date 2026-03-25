start() {
    @[straight]
    switch 1; {
        case 1; { exit 0; }
        case 2; { exit 1; }
        default { exit 2; }
    }
}

: OUTPUT
BB1: start
jmp lb{X}
BB2: lb{X}
r15 = $0;
rdx = r15;
exit rdx;
BB3: lb{X}
r15 = $1;
rdx = r15;
exit rdx;
BB4: lb{X}
r15 = $2;
rdx = r15;
exit rdx;
BB5: lb{X}
r15 = $1;
rax = r15;
cmp rax, 1;
sete al;
rdx movzx al;
cmp rdx, 0;
je lb{X}
jne lb{X}
BB6: lb{X}
r15 = $1;
rax = r15;
cmp rax, 2;
sete al;
rcx movzx al;
cmp rcx, 0;
je lb{X}
jne lb{X}
BB7: lb{X}
jmp lb{X}
BB8: lb{X}
: