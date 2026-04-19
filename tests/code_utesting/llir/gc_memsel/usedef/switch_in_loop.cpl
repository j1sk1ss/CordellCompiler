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
  B4 [label="B4:\nentry=FDCL-1\nexit=HIR_SETPOS\nHEAD\nIN={}\nDEF={11,52,51}\nUSE={75}\nOUT={}\nPREDS=0"];
  B4 -> B23 [label="fall"];
  B23 [label="B23:\nentry=PHI_PREAMBLE-1\nexit=PHI_PREAMBLE\nIN={}\nDEF={53,54}\nUSE={52,51}\nOUT={}\nPREDS=1"];
  B23 -> B5 [label="fall"];
  B5 [label="B5:\nentry=MKLB21\nexit=IFOP2\nIN={}\nDEF={12}\nUSE={53}\nOUT={}\nPREDS=2"];
  B5 -> B6 [label="fall"];
  B5 -> B7 [label="jump"];
  B6 [label="B6:\nentry=MKLB22\nexit=JMP\nIN={}\nDEF={55,53,78,56,54,13,79,76,81,16,14,15}\nUSE={53,78,81,54,77,80}\nOUT={}\nPREDS=1"];
  B6 -> B5 [label="jump"];
  B7 [label="B7:\nentry=MKLB23\nexit=FRET\nIN={}\nDEF={82}\nUSE={54}\nOUT={}\nPREDS=1"];
  B8 [label="B8:\nentry=FEND-1\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
digraph CFG_func1 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B9 [label="B9:\nentry=FDCL-1\nexit=FEND\nHEAD\nIN={}\nDEF={17,85,87,57,84,86}\nUSE={83}\nOUT={}\nPREDS=0"];
}
digraph CFG_func2 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B10 [label="B10:\nentry=FDCL-1\nexit=FEND\nHEAD\nIN={}\nDEF={19,18,89,60,92,58,91,59,20}\nUSE={88,91,90}\nOUT={}\nPREDS=0"];
}
digraph CFG_func3 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B11 [label="B11:\nentry=STRT-1\nexit=HIR_SETPOS\nHEAD\nIN={}\nDEF={66,63,23,21,95,101,61,29,26,65,28,27,64,98,100,22,97,25,24,62}\nUSE={93,97,102,96,94,100,99}\nOUT={}\nPREDS=0"];
  B11 -> B24 [label="fall"];
  B24 [label="B24:\nentry=PHI_PREAMBLE-1\nexit=PHI_PREAMBLE\nIN={}\nDEF={67,68,69}\nUSE={64,63,65}\nOUT={}\nPREDS=1"];
  B24 -> B12 [label="fall"];
  B12 [label="B12:\nentry=MKLB174\nexit=IFOP2\nIN={}\nDEF={103,30}\nUSE={67,66}\nOUT={}\nPREDS=2"];
  B12 -> B13 [label="fall"];
  B12 -> B21 [label="jump"];
  B13 [label="B13:\nentry=MKLB175\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B13 -> B17 [label="jump"];
  B14 [label="B14:\nentry=MKLB188\nexit=JMP\nIN={}\nDEF={31,70,106,32,71,74,104}\nUSE={106,69,68,105}\nOUT={}\nPREDS=1"];
  B14 -> B20 [label="jump"];
  B15 [label="B15:\nentry=MKLB202\nexit=JMP\nIN={}\nDEF={107,70,71,33,34,109,73}\nUSE={68,69,109,108}\nOUT={}\nPREDS=1"];
  B15 -> B20 [label="jump"];
  B16 [label="B16:\nentry=JMP-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B17 [label="B17:\nentry=MKLB186\nexit=IFOP2\nIN={}\nDEF={112,115,118,37,44,38,35,43,40,46,36,116,122,113,42,39,45,119,110,121,41}\nUSE={117,120,112,115,118,114,67,121,111,62}\nOUT={}\nPREDS=1"];
  B17 -> B14 [label="fall"];
  B17 -> B18 [label="jump"];
  B18 [label="B18:\nentry=MKLB236\nexit=IFOP2\nIN={}\nDEF={48,123,47}\nUSE={44}\nOUT={}\nPREDS=1"];
  B18 -> B15 [label="fall"];
  B18 -> B19 [label="jump"];
  B19 [label="B19:\nentry=MKLB240\nexit=PHI_PREAMBLE\nIN={}\nDEF={71,70}\nUSE={69,68}\nOUT={}\nPREDS=1"];
  B19 -> B20 [label="fall"];
  B20 [label="B20:\nentry=MKLB187\nexit=JMP\nIN={}\nDEF={67,50,124,68,69,49,72,126}\nUSE={67,125,70,71,126}\nOUT={}\nPREDS=3"];
  B20 -> B12 [label="jump"];
  B21 [label="B21:\nentry=MKLB176\nexit=EXITOP\nIN={}\nDEF={128,129,127}\nUSE={69,68}\nOUT={}\nPREDS=1"];
  B22 [label="B22:\nentry=HIR_STEND-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
/: