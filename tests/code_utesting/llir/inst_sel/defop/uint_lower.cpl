start() {
    u32 a = 0 as i32;
    u32 b = 0 as i32;
    u32 c = a < b;
    u32 d = a <= b;
}

:/ OUTPUT
BB1: start
%4 = $0;
%5 = %4;
%0 = %5;
%6 = $0;
%7 = %6;
%1 = %7;
%10 = %0;
cmp %10, %1;
setb al;
%8 movzx al;
%2 = %8;
%11 = %0;
cmp %11, %1;
stbe al;
%9 movzx al;
%3 = %9;
send
/: