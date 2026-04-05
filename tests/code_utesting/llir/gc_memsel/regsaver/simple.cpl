function foo(i32 a) {
    i32 b = a;
    i32 c = b + a + 10 as i32;
    return a + c;
}

start() {
    i32 a = 1 as i32;
    foo(a);
    i32 b = a + 10 as i32;
    exit b as i8;
}

:/ OUTPUT
BB2: fn foo0(i32 a)
r11 = rdi;
r10 = r11;
rax = r10;
rax = rax + r11;
rbx = rax;
r9 = $10;
rax = rbx;
rax = rax + r9;
rdx = rax;
rdi = rdx;
rax = r11;
rax = rax + rdi;
rsi = rax;
rax = rsi;
return rax;
BB3:
BB4: start
r10 = $1;
rdi = r10;
use rdi;
rdi <<= rdi;
push(r9);
push(r11);
push(rbx);
push(rdx);
push(rdi);
call foo0(i32 a);
rdi = pop();
rdx = pop();
rbx = pop();
r11 = pop();
r9 = pop();
rdx = $10;
rax = rdi;
rax = rax + rdx;
r9 = rax;
r11 = r9;
rbx = r11;
rdx = rbx;
exit rdx;
BB5:
/: