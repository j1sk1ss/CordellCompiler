function foo(f32 a, f64 b, i32 c, i32 d) {
}

start(f32 a, i32 b) {
    foo(10.0, 10.0, 1, 1);
}

:/ OUTPUT
BB2: fn foo0(f32 a, f64 b, i32 c, i32 d)
%0 = %6;
%1 = %7;
%2 = %8;
%3 = %9;
fend
BB3: start
%4 cvttss2si [rbp + 8];
%5 = [rbp + 16];
%10 <<= $4621819117588971520;
%11 <<= $4621819117588971520;
%12 <<= $1;
%13 <<= $1;
call foo0(f32 a, f64 b, i32 c, i32 d);
send
/: