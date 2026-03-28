start() {
    @[no_fall]
    @[straight]
    switch 1; {
        @[cold] case 1; { exit 1; }
        @[cold] case 2; { exit 2; }
        case 3; { exit 3; }
        default { exit 4; }
    }
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=JMP\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B5 [label="jump"];
  B2 [label="B2:\nentry=MKLB28\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B3 [label="B3:\nentry=MKLB36\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 [label="B4:\nentry=JMP-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B5 [label="B5:\nentry=MKLB10\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B5 -> B10 [label="fall"];
  B5 -> B6 [label="jump"];
  B6 [label="B6:\nentry=MKLB46\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B11 [label="fall"];
  B6 -> B7 [label="jump"];
  B7 [label="B7:\nentry=MKLB49\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B7 -> B2 [label="fall"];
  B7 -> B8 [label="jump"];
  B8 [label="B8:\nentry=MKLB52\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B8 -> B3 [label="jump"];
  B9 [label="B9:\nentry=MKLB11\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B10 [label="B10:\nentry=MKLB12\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B11 [label="B11:\nentry=MKLB20\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B12 [label="B12:\nentry=ENDSCOPE-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: