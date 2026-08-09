: BUG :
function foo<T>(T a, i32 b) -> T {
    10;
}

function foo<T>(T a, i64 b) -> T {
    20;
}

start() {
    exit (foo<i32>(10, 10 as i32) + foo<i32>(10, 10 as i64)) as u8;
}

:/ OUTPUT
id: 1, name: foo (virt: foo1), ext=0, glob=0, used=0, local=0, sid=1, generic=1
id: 3, name: foo (virt: foo3__i32), ext=0, glob=0, used=0, local=0, sid=1, generic=0
id: 0, name: foo (virt: foo0), ext=0, glob=0, used=0, local=0, sid=1, generic=1
[ENTRY] id: 2, name: _main (virt: _main), ext=0, glob=1, used=1, local=0, sid=1, generic=0
/: