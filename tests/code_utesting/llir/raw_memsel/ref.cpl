{
    start() {
        i32 b;
        ptr i32 a = ref b;
    }
}

: OUTPUT
BB{X}: start
rax = &([rbp - 8]);
[rbp - 16] = rax;
[rbp - 24] = [rbp - 16];
[rbp - 32] = [rbp - 24];
send
: