start(i32 b) {
    i32 a;
    @[no_fall] @[straight]
    switch b; {
        case 1; a = 1;
        case 2; a = 2;
        default a = 3;
    }
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
         [b] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=2)
         [switch] (SWITCH_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [b] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=1)
            { scope, id=-1 }
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=3 }
                     [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [a] (I32_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=4 }
                     [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [a] (I32_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
                        [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [default] (DEFAULT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=5 }
                     [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [a] (I32_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
                        [3] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: