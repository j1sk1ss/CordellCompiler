{
    start() {
        i8 a = 1;
        i16 b = a;
        i32 c = b;
        i64 d = c;
    }
}

: OUTPUT
BB{X}: start
%8 = $1;
%4 = %8;
%9 = %4;
%0 = %9;
%5 movsx %0;
%10 = %5;
%1 = %10;
%6 movsx %1;
%11 = %6;
%2 = %11;
%7 movsxd %2;
%12 = %7;
%3 = %12;
send
: