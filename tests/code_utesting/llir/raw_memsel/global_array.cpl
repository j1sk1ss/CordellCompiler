glob arr a[10, i32];
start() {
    a[0] = 1;
}

: OUTPUT
%0 = arr_alloc($10);
BB{X} start
r15 = &(%a);
[rbp - 8] = r15;
r15 = $0;
rax = r15;
rax = rax * 4;
r15 = rax;
[rbp - 16] = r15;
r15 = [rbp - 16];
[rbp - 24] = r15;
r15 = [rbp - 8];
rax = r15;
rax = rax + [rbp - 24];
r15 = rax;
[rbp - 32] = r15;
r15 = $1;
[rbp - 40] = r15;
r15 = [rbp - 40];
*([rbp - 32]) = r15;
send
: