{
    @[entry]
    function foo(i32 a) {
    }

    function bar(i32 a) {
        return a;
    }
}

: OUTPUT
BB{X}: fn _main0(i32 a)
%0 = strt_loadarg();
fend
BB{X}: fn bar1(i32 a)
%1 = ldparam();
return %1;
BB{X}:
: