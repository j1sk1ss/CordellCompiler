start() {
    i32 a = 1 as i32;
    ptr i32 b;
    if a == 1 as i32; {
        b = ref a;
    }
    i32 c = 1 as i32;
    b = ref c;
}

:/ OUTPUT
BB1: start
r9 = $1;
%15 = r9;
[rbp - 8] = %15;
r10 = $1;
rax = [rbp - 8];
cmp rax, r10;
sete al;
rsi movsx al;
rdi = rdx;
cmp rsi, 0;
je lb23;
jne lb21;
BB2: lb21:
rbx = &([rbp - 8]);
rdx = rbx;
rdi = rdx;
kill(9);
jmp lb23;
BB3: lb23:
r8 = $1;
%16 = r8;
[rbp - 8] = %16;
r11 = &([rbp - 8]);
rcx = r11;
kill(9);
kill(11);
send
/: