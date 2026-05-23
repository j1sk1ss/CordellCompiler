container node {
    i32 a;
    i32 b;
}

start() {
    ptr i8 buffer = 0x8000;
    ptr node nd = buffer as ptr node;
    nd.a = 0;
    nd.b = 0;
}

:/ OUTPUT
BB1: start
%4 = $32768 as u64;
%2 = %4;
%5 = %2 as u64;
%3 = %5;
%6 = %3 + 0;
%7 = $0 as i32;
*(%6) = %7;
%8 = %3 + 8;
%9 = $0 as i32;
*(%8) = %9;
send
/: