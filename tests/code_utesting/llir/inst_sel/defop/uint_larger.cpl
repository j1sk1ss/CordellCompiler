start() {
    u32 a = 0 as u32;
    u32 b = 0 as u32;
    u32 c = a > b;
    u32 d = a >= b;
}

:/ OUTPUT
BB1: start
%4 = $0;
%0 = %4;
%5 = $0;
%1 = %5;
%8 = %0;
cmp %8, %1;
seta al;
%6 movzx al;
%2 = %6;
%9 = %0;
cmp %9, %1;
stae al;
%7 movzx al;
%3 = %7;
send
/: