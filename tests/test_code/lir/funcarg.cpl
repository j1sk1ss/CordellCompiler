{
    function a() => i32;
    function b(i32 g) => i32;
    function c(i32 f = b(a()));
    start() {
        c();
    }
}

: OUTPUT
BB{X}: start
call a() -> i32;
%2 = fret();
use %2;
stparam(%2);
call b(i32 g) -> i32;
%3 = fret();
use %3;
stparam(%3);
call c(i32 f);
send
: