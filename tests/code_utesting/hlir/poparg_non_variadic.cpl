function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function pack(i32 a, i32 b) -> i32 {
    @[poparg] i32 x;
    @[poparg] i32 y;
    return x * 10 + y;
}

start() {
    i32 v = pack(3 as i32, 4 as i32);
    put_digit(v / 10);
    put_digit(v % 10);
    exit 0;
}

:/ OUTPUT
BB4: fn putc0(i8 c) -> i0
%0 = ldparam();
%7 = &(%0);
use %7;
stparam($33554436);
stparam($1);
stparam(%7);
stparam($1);
syscall;
fend
BB5: fn put_digit1(i64 x) -> i0
%1 = ldparam();
%10 = $48 as i64;
%9 = %10 + %1;
%8 = %9 as i8;
use %8;
stparam(%8);
call putc0(i8 c) -> i0;
fend
BB6: fn pack2(i32 a, i32 b) -> i32
%2 = ldparam();
%3 = ldparam();
%4 = ldparam();
%5 = ldparam();
%12 = $10 as i32;
%11 = %4 * %12;
%13 = %11 + %5;
return %13;
BB7:
BB8: start
%14 = $3 as i32;
use %14;
%15 = $4 as i32;
use %15;
stparam(%14);
stparam(%15);
call pack2(i32 a, i32 b) -> i32;
%16 = fret();
%6 = %16;
%18 = $10 as i32;
%17 = %6 / %18;
use %17;
stparam(%17);
call put_digit1(i64 x) -> i0;
%20 = $10 as i32;
%19 = %6 % %20;
use %19;
stparam(%19);
call put_digit1(i64 x) -> i0;
exit $0;
BB9:
/: