{
    start() {
        i32 a = 1;
        ptr i32 b;
        if a == 1; {
            b = ref a;
        }
        i32 c = 1;
        b = ref c;
    }
}

: OUTPUT
BB1: start
r15 = $1;
rdi = r15;
r15 = rdi;
[rbp - 8] = r15;
r8 movsxd [rbp - 8];
r15 = r8;
rax = r15;
cmp rax, $1;
sete al;
r10 movzx al;
r15 = rcx;
rbx = r15;
cmp r10, 0;
je lb11;
jne lb9;
BB2: lb9:
r15 = &([rbp - 8]);
rcx = r15;
r15 = rcx;
rdx = r15;
r15 = rdx;
rbx = r15;
kill(9);
jmp lb11;
BB3: lb11:
r15 = $1;
rsi = r15;
r15 = rsi;
[rbp - 8] = r15;
r15 = &([rbp - 8]);
r9 = r15;
r15 = r9;
rax = r15;
kill(11);
send
: