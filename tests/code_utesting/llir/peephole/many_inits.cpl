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
rcx = $1;
rax = rcx;
rsi = $1;
rax = rsi;
rdx = $1;
rax = rdx;
rbx = $1;
rax = rbx;
rdi = $1;
rax = rdi;
send
=== AFTER ===
BB1: start
rcx = $1;
rsi = $1;
rdx = $1;
rbx = $1;
rdi = $1;
rax = $1;
send
/: