{
    section(".text") {
        function foo() {
        }
        function baz() {
        }
    }

    section(".bss") {
        function bar() {
        }
    }

    start() {
    }
}

: OUTPUT
{id: 1, name: baz (virt: baz1), ext=0, glob=0, used=0, local=0, sid=1}
{[ENTRY] id: 3, name: _main (virt: _main3), ext=0, glob=-1, used=0, local=0, sid=1}
{id: 0, name: foo (virt: foo0), ext=0, glob=0, used=0, local=0, sid=1}
{id: 2, name: bar (virt: bar2), ext=0, glob=0, used=0, local=0, sid=1}
{section='.bss', [symbol=2]}
{section='.text', [symbol=1] [symbol=0]}
: