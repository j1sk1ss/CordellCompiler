{
    function foo() {
    }
    @[entry]
    function bar() {
    }
    function baz();
}

: OUTPUT
{[ENTRY] id: 1, name: _main (virt: _main1), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{id: 0, name: foo (virt: foo0), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{id: 2, name: baz (virt: baz2), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{section='.text', <<0 1 2>>}
: