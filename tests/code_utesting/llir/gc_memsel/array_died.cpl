{
    start() {
        arr a[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        if 1; exit 1;
        arr b[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
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
[unused] %2 = arr_alloc($10);
cmp $1, 0;
je lb{X};
kill(2);
jne lb{X};
BB2: lb{X}:
rax = $1;
rdx = rax;
[unused] kill(4);
[unused] kill(5);
exit rdx;
BB3: lb{X}:
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
[unused] %3 = arr_alloc($10);
kill(3);
[unused] kill(0);
send
: