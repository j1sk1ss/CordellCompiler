{
    function a() -> i32;
    function b(i32 g) -> i32;
    function c(i32 f = b(a()));
    start() {
        c();
    }
}

:/ OUTPUT
BB1: start
call a0;
%2 = fret();
use %2;
stparam(%2);
call b1;
%3 = fret();
use %3;
stparam(%3);
call c2;
send
/: