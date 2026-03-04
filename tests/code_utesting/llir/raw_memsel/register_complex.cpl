{
    #define RAX 0
    #define RBX 1
    #define RCX 2
    start() {
        @[register(RAX)] i32 a = 0;
        @[register(RBX)] i32 b = 1;
        @[register(RCX)] i32 c = 2;
    }
}

: OUTPUT
BB{X}: start
[rbp - 8] = $0;
[rbp - 16] = [rbp - 8];
[rbp - 24] = [rbp - 16];
rax = [rbp - 24];
[rbp - 32] = $1;
[rbp - 40] = [rbp - 32];
[rbp - 48] = [rbp - 40];
rbx = [rbp - 48];
[rbp - 56] = $2;
[rbp - 64] = [rbp - 56];
[rbp - 72] = [rbp - 64];
rcx = [rbp - 72];
send
: