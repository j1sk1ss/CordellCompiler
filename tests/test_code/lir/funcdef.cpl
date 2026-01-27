{
    function bar(i32 a);
    function foo() => i32 {
        return 0;
    }

    function bar(i32 a) {}
}

: OUTPUT
BB{X}: fn foo() -> i32
return $0;

BB{X}: 
BB{X}: fn bar(i32 a)
%1 = ldparam();
fend
: