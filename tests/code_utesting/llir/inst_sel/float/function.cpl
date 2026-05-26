function foo(f32 a, f64 b, i32 c, i32 d) {
}

start(f32 a, i32 b) {
    foo(10.0, 10.0, 1, 1);
}

:/ OUTPUT
BB2: fn foo0(f32 a, f64 b, i32 c, i32 d)
[SSA] %0 = %6;
[SSA] %1 = %7;
[SSA] %2 = %8;
[SSA] %3 = %9;
fend
BB3: start
%4 = [rbp + 8];
%5 = &([rbp + 16]);
push(rcx);
%10 <<= $1;
push(rdx);
%11 <<= $1;
push(rsi);
%12 <<= $4621819117588971520;
push(rdi);
%13 <<= $4621819117588971520;
call foo0(f32 a, f64 b, i32 c, i32 d);
rdi = pop();
rsi = pop();
rdx = pop();
rcx = pop();
send
/: