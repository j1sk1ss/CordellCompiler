{
    function foo() {
        function bar() {

        }

        bar();
    }
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B2 [label="B2:\nentry=FDCL-1\nexit=JMP\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B2 -> B3 [label="jump"];
  B3 [label="B3:\nentry=MKLB4\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
}
digraph CFG_func1 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B4 [label="B4:\nentry=FDCL-1\nexit=FEND\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: