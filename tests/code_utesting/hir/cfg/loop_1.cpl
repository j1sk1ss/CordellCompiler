start() {
    while 1; {
    }
}

:/ OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=HIR_SETPOS\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B2 [label="fall"];
  B2 [label="B2:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B2 -> B3 [label="fall"];
  B2 -> B4 [label="jump"];
  B3 [label="B3:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B3 -> B2 [label="jump"];
  B4 [label="B4:\nentry=MKLB{X}\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
}
/:
