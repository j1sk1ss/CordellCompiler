{
    align(8) glob i32 a;
    align(64) {
        glob i32 b;
        glob i32 c;
    }
}

: OUTPUT
{ scope, id=1 }
   { scope, id=1 }
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
         [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=1, glob)
   { scope, id=1 }
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
         [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=1, glob)
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
         [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=1, glob)
: