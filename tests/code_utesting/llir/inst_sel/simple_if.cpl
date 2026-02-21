{
    function foo() {
        i32 a = 10;
        if a; {
            exit 1;
        }
    }
}

: OUTPUT
BB{X}: fn foo0()
%2 = $10;
%1 = %2;
%3 = %1;
%0 = %3;
cmp %0, 0;
je lb10;
jne lb8;
BB{X}: lb8:
%5 = $1;
%4 = %5;
exit %4;
BB{X}: lb10:
fend
: