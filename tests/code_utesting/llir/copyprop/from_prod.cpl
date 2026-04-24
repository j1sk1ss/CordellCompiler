start() {
    i32 a = 1;
    i32 b = 2;
    i32 c = 3;
    i32 i = 0;

    while i < 10000000; {
        a = a + b * c;
        b = b + a / 3;
        c = c ^ (a - b);
        i += 1;
    }

    exit (a + b + c) as u8;
}

:/ OUTPUT
BB1: start
rdi = $1;
rdx = $2;
rbx = $3;
rcx = $0;
r8 = rdi;
r10 = rcx;
rdi = rdx;
r9 = rbx;
BB2: lb27:
rax = r10;
cmp rax, $10000000;
setl al;
rcx movsx al;
cmp rcx, 0;
je lb29;
jne lb28;
BB3: lb28:
rax = rdi;
rax = rax * r9;
rdx = rax;
rax = r8;
rax = rax + rdx;
r8 = rax;
rdx = $3;
rax movsxd r8;
rcx movsxd rdx;
push(rdx);
unknwn;
rax = rax / rcx;
rdx = pop();
rdx = rax;
rax = rdi;
rax = rax + rdx;
rdi = rax;
rax = r8;
rax = rax - rdi;
rdx = rax;
rax = r9;
rax = rax ^ rdx;
rbx = rax;
rdx = $1;
rax = r10;
rax = rax + rdx;
rcx = rax;
r8 = r8;
r10 = rcx;
rdi = rdi;
r9 = rbx;
jmp lb27;
BB4: lb29:
rax = r8;
rax = rax + rdi;
rcx = rax;
rax = rcx;
rax = rax + r9;
rcx = rax;
rcx = rcx;
rdi = rcx;
exit rdi;
BB5: send
/: