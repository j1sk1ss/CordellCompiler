start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        @[cold] case 2; { exit 2; }
        case 3; { exit 3; }
        default { exit 4; }
    }
}

:/ OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=JMP\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B5 [label="jump"];
  B2 [label="B2:\nentry=MKLB{X}\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B3 [label="B3:\nentry=MKLB{X}\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=4"];
  B4 [label="B4:\nentry=JMP-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B5 [label="B5:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B5 -> B6 [label="fall"];
  B5 -> B11 [label="jump"];
  B6 [label="B6:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B7 [label="fall"];
  B6 -> B8 [label="jump"];
  B7 [label="B7:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B7 -> B3 [label="jump"];
  B8 [label="B8:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B8 -> B9 [label="fall"];
  B8 -> B10 [label="jump"];
  B9 [label="B9:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B9 -> B3 [label="jump"];
  B10 [label="B10:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B10 -> B19 [label="jump"];
  B11 [label="B11:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B11 -> B12 [label="fall"];
  B11 -> B17 [label="jump"];
  B12 [label="B12:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B12 -> B13 [label="fall"];
  B12 -> B14 [label="jump"];
  B13 [label="B13:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B13 -> B3 [label="jump"];
  B14 [label="B14:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B14 -> B15 [label="fall"];
  B14 -> B16 [label="jump"];
  B15 [label="B15:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B15 -> B3 [label="jump"];
  B16 [label="B16:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B16 -> B2 [label="jump"];
  B17 [label="B17:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B17 -> B20 [label="jump"];
  B18 [label="B18:\nentry=MKLB{X}\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B19 [label="B19:\nentry=MKLB{X}\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B20 [label="B20:\nentry=MKLB{X}\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B21 [label="B21:\nentry=ENDSCOPE-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
/:
