{
    start() {
        if 1; {
        }
        else if 2; {
        }
        else if 3; {
        }
        else if 4; {
        }
        else {
            exit 1;
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
  B2 -> B13 [label="jump"];
  B3 [label="B3:\nentry=MKLB5\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B3 -> B4 [label="fall"];
  B3 -> B5 [label="jump"];
  B4 [label="B4:\nentry=MKLB11\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 -> B12 [label="jump"];
  B5 [label="B5:\nentry=MKLB12\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B5 -> B6 [label="fall"];
  B5 -> B7 [label="jump"];
  B6 [label="B6:\nentry=MKLB18\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B11 [label="jump"];
  B7 [label="B7:\nentry=MKLB19\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B7 -> B8 [label="fall"];
  B7 -> B9 [label="jump"];
  B8 [label="B8:\nentry=MKLB25\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B8 -> B10 [label="jump"];
  B9 [label="B9:\nentry=MKLB26\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B10 [label="B10:\nentry=MKLB27\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B10 -> B11 [label="jump"];
  B11 [label="B11:\nentry=MKLB20\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B11 -> B12 [label="jump"];
  B12 [label="B12:\nentry=MKLB13\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B12 -> B13 [label="jump"];
  B13 [label="B13:\nentry=MKLB6\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
}
: