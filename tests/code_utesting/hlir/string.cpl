{
    start() {
        ptr i8 msg = "Hello, World!";
        str ans = "Hello, User!";

        msg[0] = 'o';
        ans[1] = 'g';
    }
}

: OUTPUT
BB{X}: start
%2 = str(Hello, World!) as u64;
%0 = %2;
%1 = str_alloc(str(Hello, User!));
%3 = %0;
%4 = $0 * 1;
%6 = %4 as u64;
%5 = %3 + %6;
%7 = $111 as i8;
*(%5) = %7;
%8 = &(%1);
%9 = $1 * 1;
%11 = %9 as u64;
%10 = %8 + %11;
%12 = $103 as i8;
*(%10) = %12;
send
: