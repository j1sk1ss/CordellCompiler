#define RAX 0
#define RBX 1
#define RCX 2
start() {
    @[register(RAX)] i32 a = 0 as i32;
    @[register(RBX)] i32 b = 1 as i32;
    @[register(RCX)] i32 c = 2 as i32;
}

: OUTPUT
BB1: start
r15 = $0;
[rbp - 8] = r15;
r15 = [rbp - 8];
rax = r15;
r15 = $1;
[rbp - 16] = r15;
r15 = [rbp - 16];
rbx = r15;
r15 = $2;
[rbp - 24] = r15;
r15 = [rbp - 24];
rcx = r15;
send
: