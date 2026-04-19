: BLOCK_TEST :
: RUN_ASM :
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
LG
/:
