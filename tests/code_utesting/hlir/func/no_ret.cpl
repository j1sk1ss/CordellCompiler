{
    @[entry]
    function foo(i32 a) {
    }

    function bar(i32 a) {
        return a;
    }
}

:/ OUTPUT
BB2: fn _main(i32 a)
%0 = strt_loadarg();
fend
BB3: fn bar1(i32 a)
%1 = ldparam();
return %1;
BB4: fend
/: