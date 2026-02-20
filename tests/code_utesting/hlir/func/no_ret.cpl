{
    function foo(i32 a) {

    }

    function bar(i32 a) {
        return a;
    }
}

: OUTPUT
BB{X}: fn foo0(i32 a)
%0 = ldparam();
fend
BB{X}: fn bar1(i32 a)
%1 = strt_loadarg();
return %1;
BB{X}:
: