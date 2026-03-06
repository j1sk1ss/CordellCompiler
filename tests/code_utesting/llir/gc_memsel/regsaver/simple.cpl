{
    function foo(i32 a) {
        i32 b = a;
        i32 c = b + a + 10;
        return a + c;
    }

    start() {
        i32 a = 1;
        foo(a);
        i32 b = a + 10;
        exit b;
    }
}

: OUTPUT
BB2: fn foo0(i32 a)
rax = rdi;
r10 = rax;
rax = r10;
rbx = rax;
rax = rbx;
rax = rax;
rax = rax + r10;
rax = rax;
rsi = rax;
rcx movsxd rsi;
rax = rcx;
rax = rax;
rax = rax + $10;
rax = rax;
r9 = rax;
rax = r9;
r8 = rax;
rax = r8;
r11 = rax;
rax = r10;
rax = rax;
rax = rax + r11;
rax = rax;
rdx = rax;
rax = rdx;
rax = rax;
[unused] kill(9);
[unused] kill(23);
[unused] kill(21);
[unused] kill(29);
[unused] kill(26);
[unused] kill(28);
[unused] kill(27);
[unused] kill(32);
[unused] kill(16);
[unused] kill(5);
[unused] kill(6);
[unused] kill(15);
[unused] kill(19);
[unused] kill(8);
[unused] kill(31);
[unused] kill(14);
[unused] kill(7);
[unused] kill(30);
[unused] kill(22);
[unused] kill(33);
[unused] kill(34);
[unused] kill(25);
[unused] kill(24);
[unused] kill(20);
return rax;
BB3:
BB4: start
rax = $1;
rcx = rax;
rax = rcx;
rax = r10;
r10 = rax;
push(r9);
use r10;
push(r10);
rdi <<= rax;
push(rax);
push(rsi);
call foo0(i32 a);
push(rdx);
rax = pop();
push(r8);
r10 = pop();
r8 = pop();
r9 = pop();
rdx = pop();
rax = rdx;
rsi = pop();
rax = rax + $10;
rdx movsxd r10;
r9 = rax;
rax = rax;
rsi = rax;
rax = rax;
r8 = rax;
rax = r9;
rdx = rax;
rax = rsi;
[unused] kill(42);
rax = r8;
[unused] kill(44);
[unused] kill(17);
[unused] kill(45);
[unused] kill(36);
[unused] kill(43);
[unused] kill(39);
[unused] kill(40);
[unused] kill(38);
[unused] kill(35);
[unused] kill(13);
[unused] kill(10);
[unused] kill(37);
[unused] kill(12);
[unused] kill(18);
BB5:
+[unused] kill(11);
+[unused] kill(41);
+exit rdx;
: