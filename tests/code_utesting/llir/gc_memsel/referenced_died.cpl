start() {
    i32 a = 1 as i32;
    ptr i32 b;
    if a == 1 as i32; {
        b = ref a;
    }
    i32 c = 1 as i32;
    b = ref c;
}

: OUTPUT
BB1: start
r15 = $1;
rdi = r15;
r15 = rdi;
[rbp - 8] = r15;
r15 = $1;
r10 = r15;
r15 = [rbp - 8];
rax = r15;
cmp rax, r10;
sete al;
r9 movzx al;
r15 = rcx;
rbx = r15;
cmp r9, 0;
je lb25;
jne lb23;
BB2: lb23:
r15 = &([rbp - 8]);
rcx = r15;
r15 = rcx;
rdx = r15;
r15 = rdx;
rbx = r15;
kill(9);
jmp lb25;
BB3: lb25:
r15 = $1;
rsi = r15;
r15 = rsi;
[rbp - 8] = r15;
r15 = &([rbp - 8]);
r8 = r15;
r15 = r8;
rax = r15;
kill(9);
kill(11);
send
: