start() {
    @[cold] if 1; {
    }
    else {
    }

    @[cold] if 2; {
    }
    else {
    }

    @[cold] if 3; {
    }
    else {
    }

    exit 1;
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=IFOP2\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B8 [label="fall"];
  B1 -> B2 [label="jump"];
  B2 [label="B2:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B2 -> B3 [label="jump"];
  B3 [label="B3:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B3 -> B9 [label="fall"];
  B3 -> B4 [label="jump"];
  B4 [label="B4:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 -> B5 [label="jump"];
  B5 [label="B5:\nentry=MKLB{X}\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B5 -> B10 [label="fall"];
  B5 -> B6 [label="jump"];
  B6 [label="B6:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B7 [label="jump"];
  B7 [label="B7:\nentry=MKLB{X}\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B8 [label="B8:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B8 -> B3 [label="jump"];
  B9 [label="B9:\nentry=MKLB11\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B9 -> B5 [label="jump"];
  B10 [label="B10:\nentry=MKLB{X}\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B10 -> B7 [label="jump"];
  B11 [label="B11:\nentry=ENDSCOPE-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: