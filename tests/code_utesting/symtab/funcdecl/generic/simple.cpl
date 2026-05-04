function foo<T>(T a, T b) -> T {
}
function bar<T>() {
}

start() {
    foo<i32>(1, 1);
    foo<i8>(1, 1);
    bar<i0>();
    bar<f64>();
}

:/ OUTPUT
{id: 1, name: bar (virt: bar1), ext=0, glob=0, used=0, local=0, sid=1, generic=-1}
{id: 4, name: bar (virt: bar4__i0), ext=0, glob=0, used=0, local=0, sid=1, generic=0}
{id: 3, name: bar (virt: bar3__f64), ext=0, glob=0, used=0, local=0, sid=1, generic=0}
{id: 0, name: foo (virt: foo0), ext=0, glob=0, used=0, local=0, sid=1, generic=-1}
{id: 5, name: foo (virt: foo5__i8), ext=0, glob=0, used=0, local=0, sid=1, generic=0}
{id: 6, name: foo (virt: foo6__i32), ext=0, glob=0, used=0, local=0, sid=1, generic=0}
{[ENTRY] id: 2, name: _main (virt: _main), ext=0, glob=-1, used=0, local=0, sid=1, generic=0}
/: