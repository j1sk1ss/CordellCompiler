{
    start() {
        loop {
            if 1; {
                break;
            }
        }
    }
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=MKSCOPE\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B2 [label="fall"];
  B2 [label="B2:\nentry=MKLB4\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B2 -> B3 [label="jump"];
  B3 [label="B3:\nentry=MKLB5\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B3 -> B4 [label="fall"];
  B3 -> B6 [label="jump"];
  B4 [label="B4:\nentry=MKLB8\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 -> B7 [label="jump"];
  B5 [label="B5:\nentry=ENDSCOPE-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B6 [label="B6:\nentry=MKLB10\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B2 [label="jump"];
  B7 [label="B7:\nentry=MKLB6\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
}
: