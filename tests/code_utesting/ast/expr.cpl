start() {
   i32 a = 10 + 11 * 12;
   i32 b = 10 - 1;
   a += b += a; 
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [*] (MULTIPLY_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [11] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [12] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=2)
            [-] (MINUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
            [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
               [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
/: