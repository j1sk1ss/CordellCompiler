function foo(i32 a) {
    return a + 10 as i32;
}

start() {
    i32 a = 1 as i32;
    foo(a);
    i32 b = a + 1 as i32;
    exit b as i8;
}

:/ OUTPUT
BB2: fn foo0(i32 a)
rsi = rdi;
rdx = $10;
rax = rsi;
rax = rax + rdx;
rbx = rax;
rax = rbx;
return rax;
BB3:
BB4: start
rdi = $1;
rdx = rdi;
use rdx;
rdi <<= rdx;
push(rbx);
push(rdx);
call foo0(i32 a);
rdx = pop();
rbx = pop();
r10 = $1;
rax = rdx;
rax = rax + r10;
rbx = rax;
r11 = rbx;
r9 = r11;
rdx = r9;
exit rdx;
BB5:
/: