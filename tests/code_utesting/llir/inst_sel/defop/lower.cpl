start() {
    i32 a = (10 < 15) as i32;
}

:/ OUTPUT
BB1: start
%3 = $10;
cmp %3, $15;
setl al;
%2 = al;
%1 movsx %2;
%0 = %1;
send
/: