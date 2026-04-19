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
rcx = $1;
%15 = rcx;
[rbp - 8] = %15;
rdx = $1;
rax = [rbp - 8];
cmp rax, rdx;
sete al;
rcx movsx al;
cmp rcx, 0;
je lb23;
jne lb21;
BB2: lb21:
rcx = &([rbp - 8]);
rcx = rcx;
rcx = rcx;
kill(9);
jmp lb23;
BB3: lb23:
rcx = $1;
%16 = rcx;
[rbp - 8] = %16;
rcx = &([rbp - 8]);
rcx = rcx;
kill(9);
kill(10);
send
/: