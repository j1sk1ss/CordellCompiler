{
    function foo();
    function bar();
    function baz();
    start() {
        switch 1; {
            case 1; { foo(); }
            case 2; { bar(); }
            default { baz(); }
        }
    }
}

: OUTPUT
digraph CALL_GRAPH {
  rankdir=LR;
  node [shape=ellipse, fontname="monospace"];
  {F3 -> F1;}
  {F3 -> F2;}
  {F3 -> F0;}
}
: