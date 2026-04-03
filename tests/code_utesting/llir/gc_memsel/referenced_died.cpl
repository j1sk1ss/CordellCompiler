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
r8 = $1;
%15 = r8;
[rbp - 8] = %15;
r9 = $1;
rax = [rbp - 8];
cmp rax, r9;
sete al;
rbx movzx al;
rsi = rcx;
cmp rbx, 0;
je lb23;
jne lb21;
BB2: lb21:
rdx = &([rbp - 8]);
rcx = rdx;
rsi = rcx;
kill(9);
jmp lb23;
BB3: lb23:
rdi = $1;
%16 = rdi;
[rbp - 8] = %16;
r10 = &([rbp - 8]);
rax = r10;
kill(9);
kill(11);
send
/: