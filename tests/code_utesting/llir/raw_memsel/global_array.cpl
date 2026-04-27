glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
[rbp - 8] = $1;
%10 = &(%a);
[rbp - 16] = %10;
rax = $0;
rax = rax * 4;
%11 = rax;
[rbp - 24] = %11;
%12 movsx [rbp - 24];
[rbp - 32] movsx %12;
rax = [rbp - 16];
rax = rax + [rbp - 32];
%13 = rax;
[rbp - 40] = %13;
%14 = [rbp - 40];
%16 = [rbp - 8];
*(%15) = %17;
send
/: