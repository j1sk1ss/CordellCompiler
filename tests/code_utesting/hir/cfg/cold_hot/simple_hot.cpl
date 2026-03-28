start(i32 a) {
    i32 b = a + 10 as i32;
    @[hot] if b == 20 as i32; {
        a += 1 as i32;
        exit b;
    }
    else {
        exit a;
    }
    i32 d = b + a;
}

: OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B1 [label="B1:\nentry=STRT-1\nexit=IFOP2\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B1 -> B2 [label="fall"];
  B1 -> B4 [label="jump"];
  B2 [label="B2:\nentry=MKLB29\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B3 [label="B3:\nentry=MKLB31\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B4 [label="B4:\nentry=MKLB30\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B5 [label="B5:\nentry=ENDSCOPE-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
: