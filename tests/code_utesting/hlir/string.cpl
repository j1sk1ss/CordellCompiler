start() {
    ptr i8 msg = "Hello, World!";
    arr ans[0, i8] = "Hello, User!";

    msg[0] = 'o' as i8;
    ans[1] = 'g' as i8;
}

:/ OUTPUT
BB1: start
%0 = str(Hello, World!);
%1 = str_alloc(str(Hello, User!));
%2 = $111 as i8;
%3 = %0;
%4 = $0 * 1;
%6 = %4 as u64;
%5 = %3 + %6;
*(%5) = %2;
%7 = $103 as i8;
%8 = &(%1);
%9 = $1 * 1;
%11 = %9 as u64;
%10 = %8 + %11;
*(%10) = %7;
send
/: