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
rcx = rcx;
rcx = $1;
rcx = rcx;
rcx = $1;
rcx = rcx;
rcx = $1;
rcx = rcx;
rcx = $1;
rcx = rcx;
send
=== AFTER ===
BB1: start
rcx = $1;
rcx = $1;
rcx = $1;
rcx = $1;
rcx = $1;
send
/: