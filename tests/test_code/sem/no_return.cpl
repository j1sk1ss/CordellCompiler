{
    function foo() => i0 {}
    function fang() => i32 {
        if 1; {
            return 1;
        }
    }

    start() { exit 1; }
}

: OUTPUT
[INFO]    [3:20] Used 'Fang' as a function dragon-name!
[WARNING] [3:14] Function='fang' doesn't have the 'return' statement in all paths!
3 | function fang() => i32 
4 | {
4 |     if 1;
5 |     {
5 |         return 1;
5 |     }
4 | }
: