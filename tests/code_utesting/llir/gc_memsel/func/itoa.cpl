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
%82 = rdi;
[rbp - 8] = %82;
%83 = rsi;
[rbp - 16] = %83;
%84 = rdx;
[rbp - 24] = %84;
r11 = $0;
%85 = r11;
[rbp - 32] = %85;
BB14: %86 = [rbp - 32];
[rbp - 72] = %86;
kill(43);
kill(46);
r11 = [rbp - 8];
BB3: lb32:
r8 = $0;
rax = r11;
cmp rax, r8;
setg al;
r13 movsx al;
cmp r13, 0;
je lb34;
jne lb33;
BB4: lb33:
rsi = $10;
rdx = rdx ^ rdx;
rax = r11;
edx = rax % rsi;
%87 = edx;
[rbp - 8] = %87;
rdx = [rbp - 8];
rax = $48;
rax = rax + rdx;
rdi = rax;
r14 = &([rbp - 64]);
rax = [rbp - 72];
rax = rax * 1;
%88 movsxd rax;
[rbp - 32] = %88;
%89 = [rbp - 32];
[rbp - 80] = %89;
rax = r14;
rax = rax + [rbp - 80];
%90 = rax;
[rbp - 88] = %90;
%91 = [rbp - 88];
*(%92) = rdi;
rbx = $10;
rdx = rdx ^ rdx;
rax = r11;
rax = rax / rbx;
r9 = rax;
r12 = r9;
[rbp - 96] = $1;
rax = [rbp - 72];
rax = rax + [rbp - 96];
%93 = rax;
[rbp - 104] = %93;
r10 = [rbp - 104];
kill(11);
kill(15);
kill(17);
kill(16);
kill(16);
kill(21);
kill(20);
r11 = r12;
%94 = r10;
[rbp - 72] = %94;
jmp lb32;
BB5: lb34:
[rbp - 8] = $0;
kill(22);
rdx = [rbp - 8];
BB13: rsi = rdx;
kill(49);
rbx = [rbp - 72];
BB6: lb97:
[rbp - 8] = $0;
rax = rbx;
cmp rax, [rbp - 8];
setg al;
%95 movsx al;
[rbp - 32] = %95;
cmp [rbp - 32], 0;
je lb109;
jne lb108;
BB7: lb108:
rax = [rbp - 72];
cmp rax, [rbp - 24];
setl al;
%96 movsx al;
[rbp - 80] = %96;
rdx = [rbp - 80];
jmp lb110;
BB8: lb109:
rdx = 0;
jmp lb110;
BB9: lb110:
cmp rdx, 0;
je lb99;
jne lb98;
BB10: lb98:
%97 = &([rbp - 64]);
[rbp - 88] = %97;
rax = rbx;
rax = rax * 1;
%98 movsxd rax;
[rbp - 96] = %98;
%99 = [rbp - 96];
[rbp - 104] = %99;
rax = [rbp - 88];
rax = rax + [rbp - 104];
%100 = rax;
[rbp - 112] = %100;
%101 = [rbp - 112];
%102 = *(%101);
[rbp - 120] = %102;
%103 = [rbp - 16];
[rbp - 128] = %103;
rax = rsi;
rax = rax * 1;
%104 movsxd rax;
[rbp - 136] = %104;
%105 = [rbp - 136];
[rbp - 144] = %105;
rax = [rbp - 128];
rax = rax + [rbp - 144];
rdx = rax;
%106 = [rbp - 120];
*(rdx) = %107;
[rbp - 152] = $1;
rax = rsi;
rax = rax + [rbp - 152];
%108 = rax;
[rbp - 160] = %108;
%109 = [rbp - 160];
[rbp - 72] = %109;
[rbp - 168] = $1;
rax = rbx;
rax = rax - [rbp - 168];
%110 = rax;
[rbp - 176] = %110;
%111 = [rbp - 176];
[rbp - 184] = %111;
kill(27);
kill(28);
kill(30);
kill(29);
kill(31);
kill(32);
kill(33);
kill(35);
kill(37);
kill(36);
kill(53);
kill(39);
kill(38);
kill(54);
rbx = [rbp - 184];
rsi = [rbp - 72];
jmp lb97;
BB11: lb99:
kill(46);
kill(49);
kill(49);
kill(22);
kill(24);
kill(23);
kill(25);
kill(43);
kill(44);
kill(45);
fend
BB12: start
rbx = &(%buffer);
use rbx;
r10 = 32;
use r10;
rdi <<= $123321;
rsi <<= rbx;
rdx <<= r10;
call itoa0(i32 num, i8* buffer, i32 blen) -> i0;
r9 = &(%buffer);
use r9;
rax <<= $33554436;
push(rdi);
rdi <<= $1;
push(rsi);
rsi <<= r9;
push(rdx);
rdx <<= 32;
syscall;
rdx = pop();
rsi = pop();
rdi = pop();
send
/: