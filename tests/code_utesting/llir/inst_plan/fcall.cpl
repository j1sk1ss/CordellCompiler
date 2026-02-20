{
    function foo() -> i0;
    function bar() -> i0;
    start() {
        foo();
        i32 a = 10;
        bar();
        i32 b = a + 10;
    }
}

: OUTPUT
BB{X}: start
call {X}() -> i0;
call {X}() -> i0;
%{X} = $10 as i32;
%{X} = %{X};
%{X} = %{X} as i64;
%{X} = %{X} + $10;
%{X} = %{X} as i32;
%{X} = %{X};
send
: