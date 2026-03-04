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
r13 = $1;
r10 = r13;
r14 = r10;
[rbp - 8] = r14;
r11 movsxd [rbp - 8];
ebx = r11;
rax = ebx;
cmp rax, $1;
sete al;
eax movzx al;
rsp = rbx;
rbp = rsp;
cmp eax, 0;
je lb11;
[unused] kill(17);
[unused] kill(4);
[unused] kill(16);
[unused] kill(5);
[unused] kill(14);
[unused] kill(15);
[unused] kill(1);
[unused] kill(3);
[unused] kill(18);
[unused] kill(10);
jne lb9;
BB2: lb9:
rax = &([rbp - 8]);
rcx = rax;
rbx = rcx;
rdx = rbx;
rsi = rdx;
rbp = rsi;
[unused] kill(19);
kill(9);
[unused] kill(21);
[unused] kill(13);
[unused] kill(10);
[unused] kill(6);
[unused] kill(20);
jmp lb11;
BB3: lb11:
r8 = $1;
r9 = r8;
r15 = r9;
[rbp - 12] = r15;
rax = &([rbp - 12]);
r12 = rax;
rdi = r12;
rbx = rdi;
[unused] kill(8);
[unused] kill(12);
kill(11);
[unused] kill(23);
[unused] kill(7);
[unused] kill(25);
[unused] kill(24);
[unused] kill(22);
send
: