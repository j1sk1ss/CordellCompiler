start() {
    i32 a = ((i32 a, i32 b) => { if 1; return a + b; else return a; })(10, 10);
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=2)
         { scope, id=3 }
            [if] (IF_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               { scope, id=4 }
                  [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=4)
                     [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
                        [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
               { scope, id=3 }
                  [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=3)
                     [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
         [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: