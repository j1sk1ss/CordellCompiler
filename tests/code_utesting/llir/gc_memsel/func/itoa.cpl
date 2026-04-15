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
rbx = rdi;
r10 = rsi;
r9 = rdx;
rdx = $0;
rdx = rdx;
BB14: r8 = rdx;
rdi = rbx;
BB3: lb32:
rdx = $0;
rax = rdi;
cmp rax, rdx;
setg al;
rdx movsx al;
cmp rdx, 0;
je lb34;
jne lb33;
BB4: lb33:
rdx = $10;
rax movsxd rdi;
rcx movsxd rdx;
unknwn;
rdx = rax % rcx;
rdx = rdx;
rdx = rdx;
rax = $48;
rax = rax + rdx;
rsi = rax;
rbx = &([rbp - 32]);
rax = r8;
rax = rax * 1;
rdx movsxd rax;
rdx = rdx;
rax = rbx;
rax = rax + rdx;
rdx = rax;
*(rdx) = rsi;
rdx = $10;
rax movsxd rdi;
rcx movsxd rdx;
unknwn;
rax = rax / rcx;
rdx = rax;
rbx = rdx;
rdx = $1;
rax = r8;
rax = rax + rdx;
rdx = rax;
rdx = rdx;
rdi = rbx;
r8 = rdx;
jmp lb32;
BB5: lb34:
rdx = $0;
rdx = rdx;
BB13: rdi = rdx;
r8 = r8;
BB6: lb97:
rdx = $0;
rax = r8;
cmp rax, rdx;
setg al;
rdx movsx al;
cmp rdx, 0;
je lb109;
jne lb108;
BB7: lb108:
rax = rdi;
cmp rax, r9;
setl al;
rdx movsx al;
rdx = rdx;
jmp lb110;
BB8: lb109:
rdx = 0;
jmp lb110;
BB9: lb110:
cmp rdx, 0;
je lb99;
jne lb98;
BB10: lb98:
rbx = &([rbp - 32]);
rax = r8;
rax = rax * 1;
rdx movsxd rax;
rdx = rdx;
rax = rbx;
rax = rax + rdx;
rdx = rax;
rsi = *(rdx);
rbx = r10;
rax = rdi;
rax = rax * 1;
rdx movsxd rax;
rdx = rdx;
rax = rbx;
rax = rax + rdx;
rdx = rax;
*(rdx) = rsi;
rdx = $1;
rax = rdi;
rax = rax + rdx;
rdx = rax;
rbx = rdx;
rdx = $1;
rax = r8;
rax = rax - rdx;
rdx = rax;
rdx = rdx;
r8 = rdx;
rdi = rbx;
kill(47);
jmp lb97;
BB11: lb99:
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