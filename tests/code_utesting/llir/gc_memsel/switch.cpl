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
rdx = $0;
exit rdx;
BB3: lb20:
rdx = $1;
exit rdx;
BB4: lb28:
rdx = $2;
exit rdx;
BB5: lb10:
r8 = $1;
rax = r8;
cmp rax, 1;
sete al;
rdi = al;
cmp rdi, 0;
je lb38;
jne lb12;
BB6: lb38:
rdx = $1;
rax = rdx;
cmp rax, 2;
sete al;
rbx = al;
cmp rbx, 0;
je lb42;
jne lb20;
BB7: lb42:
jmp lb28;
BB8: lb11:
/: