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
rdx = $1;
rcx = rdx;
rdi = $1;
rcx = rdi;
rbx = $1;
rcx = rbx;
rsi = $1;
rcx = rsi;
r8 = $1;
rcx = r8;
send
=== AFTER ===
BB1: start
rdx = $1;
rdi = $1;
rbx = $1;
rsi = $1;
r8 = $1;
rcx = $1;
send
/: