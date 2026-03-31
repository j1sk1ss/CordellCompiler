glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
r15 = $1;
[rbp - 8] = r15;
r15 = &(%a);
[rbp - 16] = r15;
r15 = $0;
rax = r15;
rax = rax * 4;
r15 = rax;
[rbp - 24] = r15;
r15 = [rbp - 24];
[rbp - 32] = r15;
r15 = [rbp - 16];
rax = r15;
rax = rax + [rbp - 32];
r15 = rax;
[rbp - 40] = r15;
r15 = [rbp - 8];
*([rbp - 40]) = r15;
send
/:
