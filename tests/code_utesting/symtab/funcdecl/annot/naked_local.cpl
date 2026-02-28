{
    @[entry]
    function foo() {
        @[naked]
        function bar() {
        }
        bar();
    }
}

: OUTPUT
{id: 1, name: bar (virt: bar1), ext=0, glob=0, used=0, local=-1, naked=-1, sid=3}
{[ENTRY] id: 0, name: _main (virt: _main0), ext=0, glob=0, used=0, local=0, naked=0, sid=1}
{section='.text', 0}
: