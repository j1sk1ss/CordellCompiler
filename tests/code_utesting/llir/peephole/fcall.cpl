function foo(i32 a) -> i32 {
    return a + 100 as i32;
}

start() {
    exit foo(100 as i32);
}

:/ OUTPUT
=== BEFORE ===
BB2: fn foo0(i32 a) -> i32
rbx = rdi;
rsi = $100;
rax = rbx;
rax = rax + rsi;
rdx = rax;
rax = rdx;
return rax;
BB3:
BB4: start
rdx = $100;
use rdx;
rdi <<= rdx;
call foo0(i32 a) -> i32;
rbx = rax;
rdx = rbx;
exit rdx;
BB5:
=== AFTER ===
BB2: fn foo0(i32 a) -> i32
rbx = rdi;
rsi = $100;
rax = rdi;
rax = rax + rsi;
rdx = rax;
return rax;
BB3:
BB4: start
rdx = $100;
use rdx;
rdi <<= $100;
call foo0(i32 a) -> i32;
rbx = rax;
rdx = rax;
exit rdx;
BB5:
/: