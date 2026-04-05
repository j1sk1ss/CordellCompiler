function itoa(i32 num, ptr i8 buffer, i32 blen) -> i0 {
    if not num; {
        dref buffer = '0';
        return;
    }

    i32 i = 0;
    arr tmp[32, i8];
    while num > 0; {
        tmp[i] = '0' + (num % 10) as i8;
        num /= 10;
        i += 1;
    }

    i32 j = 0;
    while i > 0 && j < blen; {
        buffer[j] = tmp[i];
        j += 1;
        i -= 1;
    }
}

glob arr buffer[32, i8] = { 0 };

start() {
    itoa(123321, ref buffer, sizeof(buffer) as i32);
    syscall(0x2000004, 1, ref buffer, sizeof(buffer));
}

:/ OUTPUT
digraph CFG_func0 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B2 [label="B2:\nentry=FDCL-1\nexit=IFOP2\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
  B2 -> B3 [label="fall"];
  B2 -> B4 [label="jump"];
  B3 [label="B3:\nentry=MKLB19\nexit=FRET\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 [label="B4:\nentry=MKLB21\nexit=HIR_SETPOS\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B4 -> B5 [label="fall"];
  B5 [label="B5:\nentry=MKLB51\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B5 -> B6 [label="fall"];
  B5 -> B7 [label="jump"];
  B6 [label="B6:\nentry=MKLB52\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B6 -> B5 [label="jump"];
  B7 [label="B7:\nentry=MKLB53\nexit=HIR_SETPOS\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B7 -> B8 [label="fall"];
  B8 [label="B8:\nentry=MKLB117\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B8 -> B9 [label="fall"];
  B8 -> B10 [label="jump"];
  B9 [label="B9:\nentry=MKLB128\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B9 -> B11 [label="jump"];
  B10 [label="B10:\nentry=MKLB129\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B10 -> B11 [label="jump"];
  B11 [label="B11:\nentry=MKLB130\nexit=IFOP2\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=2"];
  B11 -> B12 [label="fall"];
  B11 -> B13 [label="jump"];
  B12 [label="B12:\nentry=MKLB118\nexit=JMP\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
  B12 -> B8 [label="jump"];
  B13 [label="B13:\nentry=MKLB119\nexit=FEND\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=1"];
}
digraph CFG_func1 {
  rankdir=TB;
  node [shape=box, fontname="monospace"];
  B14 [label="B14:\nentry=STRT-1\nexit=HIR_STEND\nHEAD\nIN={}\nDEF={}\nUSE={}\nOUT={}\nPREDS=0"];
}
/: