@[section(".bss")] @[align(8)] glob i32 a;
@[section(".bss")] glob i32 b;
glob i32 c;
start() {
   a += 1;
   b += 1;
   c += 1;
}

:/ OUTPUT
{ scope, id=1 }
   [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1, glob)
      [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=1, glob)
   [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1, glob)
      [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=1, glob)
   [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1, glob)
      [c] (I32_VARIABLE_TOKEN, v_id=2, t_id=-1, s_id=1, glob)
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=1, glob)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=1, glob)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [c] (I32_VARIABLE_TOKEN, v_id=2, t_id=-1, s_id=1, glob)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: