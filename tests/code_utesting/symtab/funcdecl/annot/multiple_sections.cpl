{
    @[section(".bss")]
    function foo() {
    }
    @[section(".test")]
    function bar() {
    }
    @[entry]
    function baz() {
    }
}

: OUTPUT
{id: 1, name: bar (virt: bar1), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{id: 0, name: foo (virt: foo0), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{[ENTRY] id: 2, name: _main (virt: _main2), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{section='.bss', 0}
{section='.test', 1}
{section='.text', 2}
: