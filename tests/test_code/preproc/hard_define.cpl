{
    #define int  i32
    #define pint ptr i32
    #define BASE int a = 1, pint b = 2
    #define FUNC (BASE) => int
    function foo FUNC;

    function fang(pint a, pint b) {
        return dref a;
    }
}

: OUTPUT
{
    function foo (i32 a = 1, ptr i32 b = 2) => i32;

    function fang(ptr i32 a, ptr i32 b) {
        return dref a;
    }
}
: