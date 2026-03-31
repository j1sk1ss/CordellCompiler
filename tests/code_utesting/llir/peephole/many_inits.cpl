start() {
    i32 a = 1 as i32;
    i32 b = 1 as i32;
    i32 c = 1 as i32;
    i32 d = 1 as i32;
    i32 e = 1 as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
r15 = $1;
rdx = r15;
r15 = rdx;
rax = r15;
r15 = $1;
rsi = r15;
r15 = rsi;
rax = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
rax = r15;
r15 = $1;
rbx = r15;
r15 = rbx;
rax = r15;
r15 = $1;
rdi = r15;
r15 = rdi;
rax = r15;
send
=== AFTER ===
BB1: start
rdx = $1;
rsi = $1;
rcx = $1;
rbx = $1;
rdi = $1;
rax = $1;
send
/:
