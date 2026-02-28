{
    function foo() {
        if 1; {
            function baz() {
                return 2;
            }
        }
        else {
            function bar() {
                return 1;
            }
        }

        return 0;
    }
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B3 [label="B3:\nentry=FDCL-1\nexit=IFOP2\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B3 -> B4 [label="fall"];
  B3 -> B6 [label="jump"];
  B4 [label="B4:\nentry=MKLB4\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 -> B5 [label="jump"];
  B5 [label="B5:\nentry=MKLB9\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B5 -> B8 [label="jump"];
  B6 [label="B6:\nentry=MKLB5\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B7 [label="jump"];
  B7 [label="B7:\nentry=MKLB18\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B7 -> B8 [label="jump"];
  B8 [label="B8:\nentry=MKLB6\nexit=FRET\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B9 [label="B9:\nentry=FEND-1\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
digraph CFG_func1 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B10 [label="B10:\nentry=FDCL-1\nexit=FRET\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B11 [label="B11:\nentry=FEND-1\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
digraph CFG_func2 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B12 [label="B12:\nentry=FDCL-1\nexit=FRET\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B13 [label="B13:\nentry=FEND-1\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: