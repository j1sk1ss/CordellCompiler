{
    section(".text") {
        function foo() {
            i32 a = 1;
            return a;
        }
    }

    start() {
        exit foo();
    }
}

: OUTPUT
BB{X}: fn foo0()
%1 = $1 as i32;
%0 = %1;
return %0;
BB{X}:
BB{X}: start
call foo0();
%2 = fret();
exit %2;
BB{X}:
: