function itoa(i32 num, ptr i8 buffer, i32 blen) -> i0 {
    i32 i = 0;
    arr tmp[32, i8];
    while num > 0; {
        tmp[i] = '0' + (num % 10) as i8;
        num /= 10;
        i += 1;
    }

    i32 j = 0;
    while i > 0 && j < blen; {
        buffer[j] = tmp[i];
        j += 1;
        i -= 1;
    }
}

glob arr buffer[32, i8] = { 0 };

start() {
    itoa(123321, ref buffer, sizeof(buffer) as i32);
    syscall(0x2000004, 1, ref buffer, sizeof(buffer));
}

:/ OUTPUT
%6 = arr_alloc($32);
BB2: fn itoa0(i32 num, i8* buffer, i32 blen) -> i0
rdx = rdi;
r11 = rsi;
r10 = rdx;
rcx = $0;
rcx = rcx;
r8 <<= rdx;
r9 <<= rcx;
BB3: lb25:
rdx = $0;
rax = r8;
cmp rax, rdx;
setg al;
rcx movsx al;
cmp rcx, 0;
je lb27;
jne lb26;
BB4: lb26:
rdx = $10;
rax movsxd r8;
rcx movsxd rdx;
push(rdx);
unknwn;
edx = rax % rcx;
eax = edx;
rdx = pop();
rcx = eax;
rdx = rcx;
rax = $48;
rax = rax + rdx;
rdi = rax;
rbx = &([rbp - 32]);
rax = r9;
rax = rax * 1;
rcx = rax;
rdx movsxd rcx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
*(rcx) = rdi;
rdx = $10;
rax movsxd r8;
rcx movsxd rdx;
push(rdx);
unknwn;
rax = rax / rcx;
rdx = pop();
rcx = rax;
rbx = rcx;
rdx = $1;
rax = r9;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
r8 <<= rbx;
r9 <<= rcx;
jmp lb25;
BB5: lb27:
rcx = $0;
rcx = rcx;
r8 <<= r9;
rdi <<= rcx;
BB6: lb67:
rdx = $0;
rax = r8;
cmp rax, rdx;
setg al;
rcx movsx al;
cmp rcx, 0;
je lb75;
jne lb74;
BB7: lb74:
rax = rdi;
cmp rax, r10;
setl al;
rcx movsx al;
rcx = rcx;
jmp lb76;
BB8: lb75:
rcx = 0;
jmp lb76;
BB9: lb76:
cmp rcx, 0;
je lb69;
jne lb68;
BB10: lb68:
rbx = &([rbp - 32]);
rax = r8;
rax = rax * 1;
rcx = rax;
rdx movsxd rcx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rsi = *(rcx);
rbx = r11;
rax = rdi;
rax = rax * 1;
rcx = rax;
rdx movsxd rcx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
*(rcx) = rsi;
rdx = $1;
rax = rdi;
rax = rax + rdx;
rcx = rax;
rbx = rcx;
rdx = $1;
rax = r8;
rax = rax - rdx;
rcx = rax;
rcx = rcx;
r8 <<= rcx;
rdi <<= rbx;
kill(47);
jmp lb67;
BB11: lb69:
fend
BB12: start
rcx = &(%buffer);
use rcx;
rdx = 32;
use rdx;
push(rdi);
rdi <<= $123321;
push(rsi);
rsi <<= rcx;
push(rdx);
rdx <<= rdx;
call itoa0(i32 num, i8* buffer, i32 blen) -> i0;
rdx = pop();
rsi = pop();
rdi = pop();
rdx = &(%buffer);
use rdx;
rax <<= $33554436;
push(rdi);
rdi <<= $1;
push(rsi);
rsi <<= rdx;
push(rdx);
rdx <<= 32;
push(rcx);
push(r11);
syscall;
r11 = pop();
rcx = pop();
rdx = pop();
rsi = pop();
rdi = pop();
send
/: