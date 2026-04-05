start() {
    arr a[10, i32];
    a[0] = 1 as i32;
}

:/ OUTPUT
BB1: start
[unused] %0 = arr_alloc($10);
[rbp - 48] = $1;
%8 = &([rbp - 40]);
[rbp - 56] = %8;
rax = $0;
rax = rax * 4;
%9 movsx rax;
[rbp - 64] = %9;
%10 = [rbp - 64];
[rbp - 72] = %10;
rax = [rbp - 56];
rax = rax + [rbp - 72];
%11 = rax;
[rbp - 80] = %11;
%12 = [rbp - 80];
%14 = [rbp - 48];
*(%13) = %15;
send
/: