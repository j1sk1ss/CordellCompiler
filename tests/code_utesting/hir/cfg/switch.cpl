{
    start() {
        switch 1; {
            case 1; {}
            case 2; { exit 1; }
            case 3; {}
            case 4; { break; }
            default { break; }
        }

        exit 1;
    }
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=JMP\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B9 [label="jump"];
  B2 [label="B2:\nentry=MKLB6\nexit=ENDSCOPE\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B2 -> B3 [label="fall"];
  B3 [label="B3:\nentry=MKLB9\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B4 [label="B4:\nentry=MKLB13\nexit=ENDSCOPE\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 -> B5 [label="fall"];
  B5 [label="B5:\nentry=MKLB16\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B5 -> B26 [label="jump"];
  B6 [label="B6:\nentry=ENDSCOPE-1\nexit=ENDSCOPE\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B7 [label="B7:\nentry=MKLB19\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=5"];
  B7 -> B26 [label="jump"];
  B8 [label="B8:\nentry=ENDSCOPE-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B9 [label="B9:\nentry=MKLB4\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B9 -> B10 [label="fall"];
  B9 -> B15 [label="jump"];
  B10 [label="B10:\nentry=MKLB23\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B10 -> B11 [label="fall"];
  B10 -> B12 [label="jump"];
  B11 [label="B11:\nentry=MKLB29\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B11 -> B7 [label="jump"];
  B12 [label="B12:\nentry=MKLB30\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B12 -> B13 [label="fall"];
  B12 -> B14 [label="jump"];
  B13 [label="B13:\nentry=MKLB31\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B13 -> B7 [label="jump"];
  B14 [label="B14:\nentry=MKLB32\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B14 -> B2 [label="jump"];
  B15 [label="B15:\nentry=MKLB24\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B15 -> B16 [label="fall"];
  B15 -> B25 [label="jump"];
  B16 [label="B16:\nentry=MKLB25\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B16 -> B17 [label="fall"];
  B16 -> B18 [label="jump"];
  B17 [label="B17:\nentry=MKLB39\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B17 -> B7 [label="jump"];
  B18 [label="B18:\nentry=MKLB40\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B18 -> B19 [label="fall"];
  B18 -> B24 [label="jump"];
  B19 [label="B19:\nentry=MKLB41\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B19 -> B20 [label="fall"];
  B19 -> B21 [label="jump"];
  B20 [label="B20:\nentry=MKLB47\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B20 -> B7 [label="jump"];
  B21 [label="B21:\nentry=MKLB48\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B21 -> B22 [label="fall"];
  B21 -> B23 [label="jump"];
  B22 [label="B22:\nentry=MKLB49\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B22 -> B7 [label="jump"];
  B23 [label="B23:\nentry=MKLB50\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B23 -> B5 [label="jump"];
  B24 [label="B24:\nentry=MKLB42\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B24 -> B4 [label="jump"];
  B25 [label="B25:\nentry=MKLB26\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B25 -> B3 [label="jump"];
  B26 [label="B26:\nentry=MKLB5\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B27 [label="B27:\nentry=HIR_STEND-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: