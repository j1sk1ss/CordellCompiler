{
    function foo() {
        return foo();
    }
}

: OUTPUT
digraph CALL_GRAPH {
  rankdir=LR;
  node [shape=ellipse, fontname="monospace"];
  F0 -> F0;
}
: