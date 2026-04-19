#define RAX 0
#define RBX 1
#define RCX 2
start() {
    @[register(RAX)] i32 a = 0 as i32;
    @[register(RBX)] i32 b = 1 as i32;
    @[register(RCX)] i32 c = 2 as i32;
}

:/ OUTPUT
BB1: start
[rbp - 8] = $0;
rax = [rbp - 8];
[rbp - 16] = $1;
rbx = [rbp - 16];
[rbp - 24] = $2;
rcx = [rbp - 24];
send
/: