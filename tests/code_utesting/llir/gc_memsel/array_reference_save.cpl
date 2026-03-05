{
    start() {
        arr a[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        if 1; exit 0;
        ptr i8 b = ref a;
        exit dref b;
    }
}

: OUTPUT
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
[unused] %4 = arr_alloc($10);
cmp $1, 0;
je lb{X};
[unused] kill(0);
jne lb{X};
BB2: lb{X}:
rax = $0;
rdx = rax;
[unused] kill(7);
[unused] kill(6);
exit rdx;
BB3: lb{X}:
rax = &([rbp - 16]);
rbp = rax;
rcx = rbp;
rbx = rcx;
rsi = rbx;
rdi = *(rsi);
rax = rdi;
rdx = rax;
[unused] kill(12);
[unused] kill(9);
[unused] kill(8);
[unused] kill(11);
kill(4);
[unused] kill(10);
[unused] kill(3);
[unused] kill(5);
[unused] kill(2);
exit rdx;
BB4:
: