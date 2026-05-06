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
  B4 [label="B4:\nentry=FDCL-1\nexit=HIR_SETPOS\nHEAD\nIN={}\nDEF={53,55,11,56,54}\nUSE={78}\nOUT={}\nPREDS=0"];
  B4 -> B5 [label="fall"];
  B5 [label="B5:\nentry=MKLB15\nexit=IFOP2\nIN={}\nDEF={12}\nUSE={55}\nOUT={}\nPREDS=2"];
  B5 -> B6 [label="fall"];
  B5 -> B7 [label="jump"];
  B6 [label="B6:\nentry=MKLB16\nexit=JMP\nIN={}\nDEF={55,82,56,80,13,79,58,81,16,57,14,15}\nUSE={55,56}\nOUT={}\nPREDS=1"];
  B6 -> B5 [label="jump"];
  B7 [label="B7:\nentry=MKLB17\nexit=FRET\nIN={}\nDEF={83}\nUSE={56}\nOUT={}\nPREDS=1"];
  B8 [label="B8:\nentry=FEND-1\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
digraph CFG_func1 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B9 [label="B9:\nentry=FDCL-1\nexit=FEND\nHEAD\nIN={}\nDEF={17,85,87,88,59,86}\nUSE={84}\nOUT={}\nPREDS=0"];
}
digraph CFG_func2 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B10 [label="B10:\nentry=FDCL-1\nexit=FEND\nHEAD\nIN={}\nDEF={60,18,19,62,92,91,61,90,20}\nUSE={89}\nOUT={}\nPREDS=0"];
}
digraph CFG_func3 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B11 [label="B11:\nentry=STRT-1\nexit=HIR_SETPOS\nHEAD\nIN={}\nDEF={66,93,63,23,96,21,95,29,26,65,67,28,27,71,68,94,72,64,22,70,97,69,25,74,24,73}\nUSE={98}\nOUT={}\nPREDS=0"];
  B11 -> B12 [label="fall"];
  B12 [label="B12:\nentry=MKLB121\nexit=IFOP2\nIN={}\nDEF={30,99}\nUSE={70,69}\nOUT={}\nPREDS=2"];
  B12 -> B13 [label="fall"];
  B12 -> B21 [label="jump"];
  B13 [label="B13:\nentry=MKLB122\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B13 -> B17 [label="jump"];
  B14 [label="B14:\nentry=MKLB131\nexit=JMP\nIN={}\nDEF={31,32,101,77,100}\nUSE={72}\nOUT={}\nPREDS=1"];
  B14 -> B20 [label="jump"];
  B15 [label="B15:\nentry=MKLB140\nexit=JMP\nIN={}\nDEF={102,33,76,34,103}\nUSE={71}\nOUT={}\nPREDS=1"];
  B15 -> B20 [label="jump"];
  B16 [label="B16:\nentry=JMP-1\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B16 -> B20 [label="jump"];
  B17 [label="B17:\nentry=MKLB129\nexit=IFOP2\nIN={}\nDEF={112,105,37,38,44,35,43,40,47,104,107,46,36,42,39,45,109,110,106,41,111,108}\nUSE={70,73}\nOUT={}\nPREDS=1"];
  B17 -> B14 [label="fall"];
  B17 -> B18 [label="jump"];
  B18 [label="B18:\nentry=MKLB165\nexit=IFOP2\nIN={}\nDEF={50,48,113,49}\nUSE={44}\nOUT={}\nPREDS=1"];
  B18 -> B15 [label="fall"];
  B18 -> B19 [label="jump"];
  B19 [label="B19:\nentry=MKLB170\nexit=MKLB\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B19 -> B20 [label="fall"];
  B20 [label="B20:\nentry=MKLB130\nexit=JMP\nIN={}\nDEF={70,115,75,52,51,114}\nUSE={70}\nOUT={}\nPREDS=4"];
  B20 -> B12 [label="jump"];
  B21 [label="B21:\nentry=MKLB123\nexit=EXITOP\nIN={}\nDEF={117,116,118}\nUSE={71,72}\nOUT={}\nPREDS=1"];
  B22 [label="B22:\nentry=HIR_STEND-1\nexit=HIR_STEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
/: