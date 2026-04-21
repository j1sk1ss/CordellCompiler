start() {
    arr a[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    if 1; exit 0;
    ptr i8 b = ref a;
    exit dref b;
}

:/ OUTPUT
BB1: start
[rbp - 16] = $0;
[rbp - 15] = $1;
[rbp - 14] = $2;
[rbp - 13] = $3;
[rbp - 12] = $4;
[rbp - 11] = $5;
[rbp - 10] = $6;
[rbp - 9] = $7;
[rbp - 8] = $8;
[rbp - 7] = $9;
rax = $1;
cmp rax, 0;
je lb23;
jne lb21;
BB2: lb21:
rdi = $0;
exit rdi;
BB3: lb23:
rcx = &([rbp - 16]);
rcx = rcx;
rcx = *(rcx);
rdi = rcx;
exit rdi;
BB4:
/: