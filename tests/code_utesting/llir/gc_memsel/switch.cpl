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
rdi = $0;
exit rdi;
BB3: lb20:
rdi = $1;
exit rdi;
BB4: lb28:
rdi = $2;
exit rdi;
BB5: lb10:
rcx = $1;
rax = rcx;
cmp rax, 1;
sete al;
rcx = al;
cmp rcx, 0;
je lb38;
jne lb12;
BB6: lb38:
rcx = $1;
rax = rcx;
cmp rax, 2;
sete al;
rcx = al;
cmp rcx, 0;
je lb42;
jne lb20;
BB7: lb42:
jmp lb28;
BB8: lb11:
/: