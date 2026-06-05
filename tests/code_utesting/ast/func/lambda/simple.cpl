start() {
    ptr i0 f = (i32 a, i32 b) => { a + b; };
    exit f(10, 10);
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, t_id=1, s_id=-1)
            [f] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
         { scope, id=3 }
            [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=3, s_id=2)
         [exit] (EXIT_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [()]
               [f] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=1, s_id=2)
               { scope, id=-1 }
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: