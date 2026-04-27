start() {
    arr a[10, i32];
    a[0] = 1 as i32;
}

:/ OUTPUT
BB1: start
[unused] %0 = arr_alloc($10);
[rbp - 48] = $1;
%10 = &([rbp - 40]);
[rbp - 56] = %10;
rax = $0;
rax = rax * 4;
%11 = rax;
[rbp - 64] = %11;
%12 movsx [rbp - 64];
[rbp - 72] movsx %12;
rax = [rbp - 56];
rax = rax + [rbp - 72];
%13 = rax;
[rbp - 80] = %13;
%14 = [rbp - 80];
%16 = [rbp - 48];
*(%15) = %17;
send
/: