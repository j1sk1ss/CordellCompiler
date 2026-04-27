function foo(i32 a) -> i32 {
    return a + 100 as i32;
}

start() {
    exit foo(100 as i32);
}

:/ OUTPUT
=== BEFORE ===
BB2: fn foo0(i32 a) -> i32
rcx = rdi;
rdx = $100;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rax = rcx;
return rax;
BB3: fend
BB4: start
rdx = $100;
use rdx;
push(rdi);
rdi <<= rdx;
call foo0(i32 a) -> i32;
rdi = pop();
rcx = rax;
rdi = rcx;
exit rdi;
BB5: send
=== AFTER ===
BB2: fn foo0(i32 a) -> i32
rcx = rdi;
rdx = $100;
rcx = rcx + rdx;
rax = rcx;
return rax;
BB3: fend
BB4: start
rdx = $100;
use rdx;
push(rdi);
rdi <<= rdx;
call foo0(i32 a) -> i32;
rdi = pop();
rcx = rax;
rdi = rcx;
exit rdi;
BB5: send
/: