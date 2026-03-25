@[entry]
function foo() -> i32 {
    @[cold] if 1; {
        return 2;
    }
    else {
    }
    return 0;
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=FDCL-1\nexit=IFOP2\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B4 [label="fall"];
  B1 -> B2 [label="jump"];
  B2 [label="B2:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B2 -> B3 [label="jump"];
  B3 [label="B3:\nentry=MKLB{X}\nexit=FRET\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 [label="B4:\nentry=MKLB{X}\nexit=FRET\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B5 [label="B5:\nentry=ENDSCOPE-1\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: