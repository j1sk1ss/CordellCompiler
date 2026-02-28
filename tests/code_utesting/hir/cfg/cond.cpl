{
    start() {
        if 1; {
        }
        else {
        }
    }
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=IFOP2\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B2 [label="fall"];
  B1 -> B3 [label="jump"];
  B2 [label="B2:\nentry=MKLB4\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B2 -> B4 [label="jump"];
  B3 [label="B3:\nentry=MKLB5\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B3 -> B4 [label="jump"];
  B4 [label="B4:\nentry=MKLB6\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
}
: