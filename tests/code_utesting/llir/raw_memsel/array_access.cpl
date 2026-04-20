start() {
    arr a[10, i32];
    a[0] = 1 as i32;
}

:/ OUTPUT
BB1: start
[unused] %0 = arr_alloc($10);
[rbp - 48] = $1;
%12 = &([rbp - 40]);
[rbp - 56] = %12;
rax = $0;
rax = rax * 4;
%13 = rax;
[rbp - 64] = %13;
%14 movsx [rbp - 64];
[rbp - 72] movsx %14;
rax = [rbp - 56];
rax = rax + [rbp - 72];
%15 = rax;
[rbp - 80] = %15;
%16 = [rbp - 80];
%18 = [rbp - 48];
*(%17) = %19;
send
/: