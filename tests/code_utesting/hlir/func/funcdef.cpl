{
    @[entry]
    function bar(i32 a);
    function foo() -> i32 {
        return 0;
    }

    function bar(i32 a) {}
}

:/ OUTPUT
BB2: fn foo1() -> i32
%2 = $0 as i32;
return %2;
BB3: fend
BB4: fn _main(i32 a)
%1 = strt_loadarg();
fend
/: