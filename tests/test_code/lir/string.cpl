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
%3 = $111 as u64;
%4 = %0;
%5 = $0 * 1;
%7 = %5 as u64;
%6 = %4 + %7;
*(%6) = %3;
%8 = $103 as i8;
%9 = &(%1);
%10 = $1 * 1;
%12 = %10 as u64;
%11 = %9 + %12;
*(%11) = %8;
send
: