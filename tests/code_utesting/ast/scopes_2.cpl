glob i32 a = 0;
function foo(i32 a) {
   a += 1;
}

:/ OUTPUT
{ scope, id=1 }
   [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1, glob)
      [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=1, glob)
      [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=2)
         { scope, id=3 }
            [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (I32_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: