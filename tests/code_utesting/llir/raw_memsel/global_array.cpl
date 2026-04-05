glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
[rbp - 8] = $1;
%8 = &(%a);
[rbp - 16] = %8;
rax = $0;
rax = rax * 4;
%9 movsx rax;
[rbp - 24] = %9;
%10 = [rbp - 24];
[rbp - 32] = %10;
rax = [rbp - 16];
rax = rax + [rbp - 32];
%11 = rax;
[rbp - 40] = %11;
%12 = [rbp - 40];
%14 = [rbp - 8];
*(%13) = %15;
send
/: