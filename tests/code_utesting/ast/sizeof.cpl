start() {
    i32 a = sizeof(1);
    arr b[10, i32];
    i32 l = sizeof(b) / sizeof(b[0]);
    i32 l2 = sizeof("Hello world!");
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [sizeof] (SIZEOF_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [arr] (ARRAY_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [b] (ARR_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
            [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [l] (I32_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [/] (DIVIDE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [sizeof] (SIZEOF_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [b] (ARR_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
                  [sizeof] (SIZEOF_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [[]]
                        [b] (ARR_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=2)
                        [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=4, s_id=-1)
            [l2] (I32_VARIABLE_TOKEN, v_id=3, t_id=0, s_id=2)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [sizeof] (SIZEOF_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [Hello world!] (STRING_VALUE_TOKEN, v_id=0, t_id=-1, s_id=-1)
/: