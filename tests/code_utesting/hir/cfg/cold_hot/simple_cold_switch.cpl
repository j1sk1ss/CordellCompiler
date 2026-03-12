start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        default { exit 2; }
    }
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=JMP\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B4 [label="jump"];
  B2 [label="B2:\nentry=MKLB10\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B3 [label="B3:\nentry=JMP-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B4 [label="B4:\nentry=MKLB4\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 -> B5 [label="fall"];
  B4 -> B6 [label="jump"];
  B5 [label="B5:\nentry=MKLB15\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B5 -> B2 [label="jump"];
  B6 [label="B6:\nentry=MKLB16\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B7 [label="fall"];
  B6 -> B8 [label="jump"];
  B7 [label="B7:\nentry=MKLB17\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B7 -> B2 [label="jump"];
  B8 [label="B8:\nentry=MKLB18\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B8 -> B10 [label="jump"];
  B9 [label="B9:\nentry=MKLB5\nexit=ENDSCOPE\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B10 [label="B10:\nentry=MKLB6\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B11 [label="B11:\nentry=ENDSCOPE-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: