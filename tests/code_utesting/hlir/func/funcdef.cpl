{
    function bar(i32 a);
    function foo() -> i32 {
        return 0;
    }

    function bar(i32 a) {}
}

: OUTPUT
BB{X}: fn foo1() -> i32
return $0;

BB{X}: 
BB{X}: fn bar0(i32 a)
%1 = strt_loadarg();
fend
: