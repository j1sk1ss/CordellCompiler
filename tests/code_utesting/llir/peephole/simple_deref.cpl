start() {
    ptr i32 a = 0xB8400;
    dref a = 0 as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rcx = $754688;
rcx = rcx;
rcx = $0;
*(rcx) = rcx;
send
=== AFTER ===
BB1: start
rcx = $754688;
rcx = rcx ^ rcx;
*(rcx) = rcx;
send
/: