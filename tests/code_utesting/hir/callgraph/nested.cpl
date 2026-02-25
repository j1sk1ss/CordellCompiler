{
    function foo();
    function bar() {
        foo();
    }
    function baz() {
        bar();
    }
    start() {
        baz();
    }
}

: OUTPUT
digraph CALL_GRAPH {
  rankdir=LR;
  node [shape=ellipse, fontname="monospace"];
  {F3 -> F2;}
  {F1 -> F0;}
  {F2 -> F1;}
}
: