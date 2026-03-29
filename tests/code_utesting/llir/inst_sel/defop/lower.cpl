start() {
    i32 a = (10 < 15) as i32;
}

:/ OUTPUT
BB1: start
%4 = $10;
%3 = %4;
cmp %3, $15;
setl al;
%2 movzx al;
%1 movsx %2;
%5 = %1;
%0 = %5;
send
/: