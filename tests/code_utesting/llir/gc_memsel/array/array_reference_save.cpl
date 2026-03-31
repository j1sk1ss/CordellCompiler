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
cmp $1, 0;
je lb{X}
jne lb{X}
BB2: lb{X}
r15 = $0;
rdx = r15;
exit rdx;
BB3: lb{X}
r15 = &([rbp - 16]);
rdx = r15;
r15 = rdx;
rax = r15;
r15 = rax;
rcx = *(r15);
r15 = rcx;
rdx = r15;
kill(4);
exit rdx;
BB4:
/:
