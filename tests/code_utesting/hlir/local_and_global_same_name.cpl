function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function foo() -> i0 {
    putc('G');
}

function bar() -> i0 {
    function foo() -> i0 {
        putc('L');
    }
    foo();
}

start() {
    bar();
    foo();
    exit 0;
}

:/ OUTPUT
BB5: fn putc0(i8 c) -> i0
%0 = ldparam();
%1 = &(%0);
use %1;
stparam($33554436);
stparam($1);
stparam(%1);
stparam($1);
syscall;
fend
BB6: fn foo1() -> i0
stparam($71);
call putc0(i8 c) -> i0;
fend
BB7: fn bar2() -> i0
jmp lb47;
BB8: lb47:
call foo3() -> i0;
fend
BB9: fn foo3() -> i0
stparam($76);
call putc0(i8 c) -> i0;
fend
BB10: start
call bar2() -> i0;
call foo1() -> i0;
exit $0;
BB11:
/: