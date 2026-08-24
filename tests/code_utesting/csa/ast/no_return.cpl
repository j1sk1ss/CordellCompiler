function foo() -> i0 {}
function fang() -> i32 {
    if 1; {
        return 1;
    }
}

start() { exit 1; }

:/ OUTPUT
[INFO]    [{X}2:16] Used 'Fang' as a function dragon-name, and now let's play some guitar songs, I guess? Also, this type of a warning is pretty annoing, isn't it? But why did you call a function with such a name?
[WARNING] [{X}2:10] Function 'fang' doesn't have the 'return' keyword on all paths!
2 | function fang() -> i32
3 | {
3 |     if 1;
4 |     {
4 |         return 1;
4 |     }
3 | }
/: