function foo(f32 a) -> f32;
start() {
    f32 a = foo(10.0);
}

:/ OUTPUT
BB1: start
push(rdi);
%3 <<= $4621819117588971520;
call foo0(f32 a) -> f32;
rdi = pop();
%2 = %4;
%1 = %2;
send
/: