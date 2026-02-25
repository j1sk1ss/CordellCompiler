{
    function foo();
    function bar() {
        if 1; {
            foo();
        }
        return 1;
    }
}

: OUTPUT
digraph CALL_GRAPH {
  rankdir=LR;
  node [shape=ellipse, fontname="monospace"];
  F1 -> F0;
}
: