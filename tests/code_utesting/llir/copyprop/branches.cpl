start() {
    i32 a = 1;
    i32 b;
    switch a; {
        case 1; {
            b = 10;
            i32 c = b + 10;
            exit c;
        }
        default {
            exit b;
        }
    }
}

:/ OUTPUT
BB1: start
rcx = $1;
rdx = rcx;
jmp lb14;
BB2: lb16:
rcx = $10;
rdx = $10;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rdi = rcx;
exit rdi;
BB3: lb32:
rdi = rsi;
exit rdi;
BB4: lb14:
rbx = 1;
rcx movsxd rdx;
rax = rcx;
cmp rax, rbx;
setl al;
rcx = al;
cmp rcx, 0;
je lb40;
jne lb39;
BB5: lb39:
jmp lb32;
BB6: lb40:
rcx movsxd rdx;
rax = rcx;
cmp rax, rbx;
setg al;
rcx = al;
cmp rcx, 0;
je lb42;
jne lb41;
BB7: lb41:
jmp lb32;
BB8: lb42:
jmp lb16;
BB9: lb15:
send
/: