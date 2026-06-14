start() {
    ptr i0 twice_plus = (i64 a, i64 b) => {
        i64 t = a * 2;
        return t + b;
    };

    exit twice_plus(8, 3) as u8;
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, t_id=5, s_id=-1)
            [twice_plus] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [lambda]
                  { scope, id=3 }
                     [i64] (I64_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                        [a] (I64_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3)
                     [i64] (I64_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                        [b] (I64_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=3)
                     { scope, id=4 }
                        [i64] (I64_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
                           [t] (I64_VARIABLE_TOKEN, v_id=3, t_id=0, s_id=4)
                           [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                              [*] (MULTIPLY_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                                 [a] (I64_VARIABLE_TOKEN, v_id=1, t_id=1, s_id=3)
                                 [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=4)
                           [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                              [t] (I64_VARIABLE_TOKEN, v_id=3, t_id=3, s_id=4)
                              [b] (I64_VARIABLE_TOKEN, v_id=2, t_id=2, s_id=3)
         [exit] (EXIT_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [u8] (U8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [()]
                  [twice_plus] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=5, s_id=2)
                  { scope, id=-1 }
                     [8] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [3] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: