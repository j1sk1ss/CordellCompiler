{
    function foo() -> i32;
    start() {
        syscall(0, foo(), 0 + 0, 0);
    }
}

:/ OUTPUT
BB1: start
call foo0;
%0 = fret();
use %0;
%1 = $0 + $0;
use %1;
stparam($0);
stparam(%1);
stparam(%0);
stparam($0);
syscall;
send
/: