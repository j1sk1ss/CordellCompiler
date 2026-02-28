{
    function foo();
    function bar();
    function baz();
    start() {
        foo(); bar(); baz();
    }
}

: OUTPUT
digraph CALL_GRAPH {
  rankdir=LR;
  node [shape=ellipse, fontname="monospace"];
  {F3 -> F0;}
  {F3 -> F1;}
  {F3 -> F2;}
}
: