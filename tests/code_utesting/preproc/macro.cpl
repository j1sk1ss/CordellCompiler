{
    #define TEST 0
    function foo(i32 a = TEST) {
    }

    #ifndef TEST2
    #define TEST2 1
    function foo2(i32 a = TEST2) {
    }
    #endif
    #undef TEST2

    #ifdef TEST2
    function foo3(i32 a) {}
    #endif
}

: OUTPUT
{
    function foo(i32 a = 0) {
    }

    function foo2(i32 a = 1) {
    }
}
: