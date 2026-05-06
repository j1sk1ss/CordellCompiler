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
jmp lb8;
BB2: lb10:
rdi = $0;
exit rdi;
BB3: lb15:
rdi = $1;
exit rdi;
BB4: lb20:
rdi = $2;
exit rdi;
BB5: lb8:
rdx = 1;
rcx = $1;
rax = rcx;
cmp rax, rdx;
sete al;
rcx = al;
cmp rcx, 0;
je lb27;
jne lb10;
BB6: lb27:
rdx = 2;
rcx = $1;
rax = rcx;
cmp rax, rdx;
sete al;
rcx = al;
cmp rcx, 0;
je lb32;
jne lb15;
BB7: lb32:
jmp lb20;
BB8: lb9:
send
/: