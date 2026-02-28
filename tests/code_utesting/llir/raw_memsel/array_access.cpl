{
    start() {
        arr a[10, i32];
        a[0] = 1;
    }
}

: OUTPUT
BB{X}: start
[unused] %0 = arr_alloc(X);
rax = &(%0);
[rbp - 48] = rax;
[rbp - 56] = $0;
rax = [rbp - 56];
rax = rax * 4;
[rbp - 64] = rax;
[rbp - 72] = [rbp - 64];
[rbp - 80] = [rbp - 72];
[rbp - 88] = [rbp - 80];
[rbp - 96] = [rbp - 48];
rax = [rbp - 96];
rax = rax + [rbp - 88];
[rbp - 104] = rax;
[rbp - 112] = [rbp - 104];
[rbp - 120] = $1;
[rbp - 128] = [rbp - 120];
[rbp - 136] = [rbp - 128];
*([rbp - 112]) = [rbp - 136];
send
: