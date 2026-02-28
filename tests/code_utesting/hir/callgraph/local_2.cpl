{
    function baz();
    function foo() {
        function bar() {
            baz();
            foo();
        }
    }
}

: OUTPUT
digraph CALL_GRAPH {
  rankdir=LR;
  node [shape=ellipse, fontname="monospace"];
  {F2 -> F1;}
  {F2 -> F0;}
}
: