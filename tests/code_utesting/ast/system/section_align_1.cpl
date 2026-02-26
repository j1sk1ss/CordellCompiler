{
    section(".bss") {
        align(8) {
            glob i32 a;
        }
        glob i32 b;
    }
    glob i32 c;
    start() {
        a += 1;
        b += 1;
        c += 1;
    }
}

: OUTPUT
{ scope, id=1 }
   { scope, id=1 }
      { scope, id=1 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=1, glob)
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
         [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=1, glob)
   [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
      [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=1, glob)
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=1, glob)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=1, glob)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
            [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=1, glob)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: