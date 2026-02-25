{
    function foo();
    function bar() {
        foo();
    }
    function baz() {
        foo();
        bar();
        bar();
    }
    function fang() {
        baz();
        bar();
        foo();
    }
    start() {
        fang();
        baz();
        foo();
    }
}

: OUTPUT
digraph CALL_GRAPH {
  rankdir=LR;
  node [shape=ellipse, fontname="monospace"];
  {F3 -> F2;}
  {F3 -> F1;}
  {F3 -> F0;}
  {F4 -> F3;}
  {F4 -> F2;}
  {F4 -> F0;}
  {F1 -> F0;}
  {F2 -> F1;}
  {F2 -> F0;}
}
: