glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
[rbp - 8] = $1;
%12 = &(%a);
[rbp - 16] = %12;
rax = $0;
rax = rax * 4;
%13 movsxd rax;
[rbp - 24] = %13;
%14 = [rbp - 24];
[rbp - 32] = %14;
rax = [rbp - 16];
rax = rax + [rbp - 32];
%15 = rax;
[rbp - 40] = %15;
%16 = [rbp - 40];
%18 = [rbp - 8];
*(%17) = %19;
send
/: