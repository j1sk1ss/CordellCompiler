function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }
    return l;
}

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

start(i32 argc, ptr ptr i8 argv) {
    i32 i = 0;
    i32 l = 0;
    i32 r = 0;
    i32 n = strlen(argv[1]);

    while i < n; {
        @[no_fall]
        @[straight]
        switch argv[1][i]; {
            case '['; {
                l += 1;
            }
            case ']'; {
                r += 1;
            }
        }
        i += 1;
    }

    putdigit(l);
    putdigit(r);
    exit 0;
}

:/ OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B4 [label="B4:\nentry=FDCL-1\nexit=HIR_SETPOS\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B4 -> B5 [label="fall"];
  B5 [label="B5:\nentry=MKLB15\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B5 -> B6 [label="fall"];
  B5 -> B7 [label="jump"];
  B6 [label="B6:\nentry=MKLB16\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B5 [label="jump"];
  B7 [label="B7:\nentry=MKLB17\nexit=FRET\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B8 [label="B8:\nentry=FEND-1\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
digraph CFG_func1 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B9 [label="B9:\nentry=FDCL-1\nexit=FEND\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
digraph CFG_func2 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B10 [label="B10:\nentry=FDCL-1\nexit=FEND\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
digraph CFG_func3 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B11 [label="B11:\nentry=STRT-1\nexit=HIR_SETPOS\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B11 -> B12 [label="fall"];
  B12 [label="B12:\nentry=MKLB121\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B12 -> B13 [label="fall"];
  B12 -> B21 [label="jump"];
  B13 [label="B13:\nentry=MKLB122\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B13 -> B17 [label="jump"];
  B14 [label="B14:\nentry=MKLB131\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B14 -> B20 [label="jump"];
  B15 [label="B15:\nentry=MKLB140\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B15 -> B20 [label="jump"];
  B16 [label="B16:\nentry=JMP-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B17 [label="B17:\nentry=MKLB129\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B17 -> B14 [label="fall"];
  B17 -> B18 [label="jump"];
  B18 [label="B18:\nentry=MKLB165\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B18 -> B15 [label="fall"];
  B18 -> B19 [label="jump"];
  B19 [label="B19:\nentry=MKLB169\nexit=MKLB\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B19 -> B20 [label="fall"];
  B20 [label="B20:\nentry=MKLB130\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=3"];
  B20 -> B12 [label="jump"];
  B21 [label="B21:\nentry=MKLB123\nexit=EXITOP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B22 [label="B22:\nentry=HIR_STEND-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
/: