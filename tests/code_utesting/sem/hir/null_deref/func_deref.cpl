function foo(ptr i8 msg) {
    dref msg = 0 as i8;
}

function bar(ptr i8 msg) {
    dref msg = 0 as i8;
}

start() {
    foo(0);
    bar(0);
    bar(1);
}

: OUTPUT
[WARNING] [2{X}] NULL-dereference error (variable 'msg' is NULL)!
: