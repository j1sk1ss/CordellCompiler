start() {
    ptr i32 a = 0xB8400;
    dref a = 0 as i32;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rbx = $754688;
rdx = rbx;
rcx = $0;
*(rdx) = rcx;
send
=== AFTER ===
BB1: start
rbx = $754688;
rdx = $754688;
rcx = rcx ^ rcx;
*(rdx) = rcx;
send
/: