{
    function foo(i32 a) {
    }
    function foo(i64 a) {
    }
    @[entry]
    function foo(i8 a) {
    }
}

: OUTPUT
{id: 1, name: foo (virt: foo1), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{id: 0, name: foo (virt: foo0), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{[ENTRY] id: 2, name: foo (virt: _main), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{section='.text', <<0 1 2>>}
: